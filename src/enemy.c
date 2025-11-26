#include "enemy.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>       
#include <time.h>       
#include <collision.h>
#include <projectile.h> 

#ifndef RADIAN_FACTOR
#define RADIAN_FACTOR (3.14159265359 / 180.0)
#endif

Enemy enemies[MAX_ENEMIES];
// Models dos inimigos (o mesmo do tank do jogador)
ObjModel enemyHullModel;
ObjModel enemyTurretModel;
ObjModel enemyPipeModel;

// Distância mínima para ativar a repulsão entre inimigos
#define SEPARATION_DIST 4.0f 

void initEnemies() {
    if (!loadOBJ("objects/hull.obj", "objects/hull.mtl", &enemyHullModel)) {
        printf("Erro ao carregar Base do inimigo.\n");
    }
    if (!loadOBJ("objects/turret.obj", "objects/turret.mtl", &enemyTurretModel)) {
        printf("Erro ao carregar Torre do inimigo.\n");
    }
    if (!loadOBJ("objects/pipe.obj", "objects/pipe.mtl", &enemyPipeModel)) {
        printf("Erro ao carregar Canhão do inimigo.\n");
    }

    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].x = (rand() % 50) - 25; 
        enemies[i].z = (rand() % 50) - 25;
        enemies[i].hullAngle = rand() % 360;   
        enemies[i].turretAngle = 0.0f;         
        enemies[i].wanderTimer = 0;
        enemies[i].targetWanderAngle = enemies[i].hullAngle;
        enemies[i].lastShootTime = 0; 
    }
}

void drawVisionCone(Enemy *e) {
    glPushAttrib(GL_ENABLE_BIT); 

    float totalAngle = e->hullAngle + e->turretAngle;
    float rad = totalAngle * RADIAN_FACTOR;
    float halfFovRad = (ENEMY_VIEW_ANGLE / 2.0f) * RADIAN_FACTOR;

    float y = (ENEMY_PIPE_Y_MIN + ENEMY_PIPE_Y_MAX) / 2.0f;
    float cx = -sinf(rad) * ENEMY_VIEW_RANGE;
    float cz = -cosf(rad) * ENEMY_VIEW_RANGE;
    float lx = -sinf(rad + halfFovRad) * ENEMY_VIEW_RANGE;
    float lz = -cosf(rad + halfFovRad) * ENEMY_VIEW_RANGE;
    float rx = -sinf(rad - halfFovRad) * ENEMY_VIEW_RANGE;
    float rz = -cosf(rad - halfFovRad) * ENEMY_VIEW_RANGE;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f);
    
    glColor3f(0.0f, 1.0f, 1.0f); 

    glBegin(GL_LINES);
        glVertex3f(e->x, y, e->z);
        glVertex3f(e->x + cx, y, e->z + cz);
        glVertex3f(e->x, y, e->z);
        glVertex3f(e->x + lx, y, e->z + lz);
        glVertex3f(e->x, y, e->z);
        glVertex3f(e->x + rx, y, e->z + rz);
        glVertex3f(e->x + lx, y, e->z + lz);
        glVertex3f(e->x + rx, y, e->z + rz);
    glEnd();

    glPopAttrib(); 
}

int canSeePlayer(Enemy *e, float px, float pz) {
    float dx = px - e->x;
    float dz = pz - e->z;
    float distSq = dx*dx + dz*dz;

    if (distSq > ENEMY_VIEW_RANGE * ENEMY_VIEW_RANGE) return 0; 

    float totalAngle = e->hullAngle + e->turretAngle;
    float rad = totalAngle * RADIAN_FACTOR;
    float forwardX = -sinf(rad);
    float forwardZ = -cosf(rad);

    float dist = sqrtf(distSq);
    float dirToPlayerX = dx / dist;
    float dirToPlayerZ = dz / dist;

    float dot = forwardX * dirToPlayerX + forwardZ * dirToPlayerZ;
    float cosHalfFov = cosf((ENEMY_VIEW_ANGLE / 2.0f) * RADIAN_FACTOR);

    return (dot > cosHalfFov);
}

void drawSingleEnemy(Enemy *e) {
    drawVisionCone(e);

    glColor3f(1.0f, 1.0f, 1.0f); 

    glPushMatrix();
        glTranslatef(e->x, 0.5f, e->z); 
        glPushMatrix();
            glRotatef(e->hullAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&enemyHullModel);
        glPopMatrix();

        glPushMatrix();
            glRotatef(e->hullAngle + e->turretAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&enemyTurretModel);
            drawModel(&enemyPipeModel);
        glPopMatrix();
    glPopMatrix();
}

void drawEnemies() {
    glEnable(GL_TEXTURE_2D); 
    glPushMatrix();
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
             drawSingleEnemy(&enemies[i]);
        }
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void updateEnemies(float playerX, float playerZ) {
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;

        float nextX = enemies[i].x;
        float nextZ = enemies[i].z;
        float currentMoveSpeed = ENEMY_SPEED;
        float targetGlobalAngle = 0.0f; 
        int shouldMove = 1; // Flag para controlar se deve andar ou só girar
        
        int seesPlayer = canSeePlayer(&enemies[i], playerX, playerZ);
        
        // 1. DECISÃO DE OBJETIVO
        if (seesPlayer) {
            if (now - enemies[i].lastShootTime > ENEMY_SHOOT_DELAY) {
                shootEnemyBullet(&enemies[i]);
                enemies[i].lastShootTime = now;
            }

            float dx = playerX - enemies[i].x;
            float dz = playerZ - enemies[i].z;
            
            // --- NOVO: VERIFICAÇÃO DE DISTÂNCIA DE COMBATE ---
            float distSq = dx*dx + dz*dz;
            if (distSq < ENEMY_MIN_COMBAT_DIST * ENEMY_MIN_COMBAT_DIST) {
                shouldMove = 0; // Está perto o suficiente, para de andar!
            }
            // ------------------------------------------------

            float targetRad = atan2f(-dx, -dz); 
            targetGlobalAngle = targetRad * (180.0f / 3.14159f);

        } else {
            enemies[i].wanderTimer--;
            if (enemies[i].wanderTimer <= 0) {
                enemies[i].wanderTimer = 60 + (rand() % 120); 
                
                float dx = playerX - enemies[i].x;
                float dz = playerZ - enemies[i].z;
                float trueAngleRad = atan2f(-dx, -dz);
                float trueAngleDeg = trueAngleRad * (180.0f / 3.14159f);

                int spread = (int)(360.0f * (1.0f - ENEMY_TRACKING_BIAS));
                
                if (spread > 0) {
                    int randomOffset = (rand() % spread) - (spread / 2);
                    enemies[i].targetWanderAngle = trueAngleDeg + randomOffset;
                } else {
                    enemies[i].targetWanderAngle = trueAngleDeg;
                }
            }
            targetGlobalAngle = enemies[i].targetWanderAngle;
        }

        // 2. INTELIGÊNCIA DE REPULSÃO (EVITAR COLISÃO ENTRE INIMIGOS)
        int isAvoiding = 0;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (i == j || !enemies[j].alive) continue;

            float dx = enemies[j].x - enemies[i].x;
            float dz = enemies[j].z - enemies[i].z;
            float distSq = dx*dx + dz*dz;

            if (distSq < SEPARATION_DIST * SEPARATION_DIST) {
                float angleToNeighborRad = atan2f(-dx, -dz);
                float angleAwayDeg = (angleToNeighborRad * (180.0f / 3.14159f)) + 180.0f;

                targetGlobalAngle = angleAwayDeg;
                isAvoiding = 1;
                shouldMove = 1; // Se está desviando, DEVE se mover para sair de perto
                
                break; 
            }
        }

        // 3. ROTAÇÃO SUAVE DO CORPO (HULL)
        // Isso acontece MESMO se shouldMove for 0 (ele gira parado)
        float hullDiff = targetGlobalAngle - enemies[i].hullAngle;
        while (hullDiff > 180) hullDiff -= 360;
        while (hullDiff < -180) hullDiff += 360;
        
        float rotSpeed = isAvoiding ? ENEMY_ROT_SPEED * 2.0f : ENEMY_ROT_SPEED;

        if (hullDiff > rotSpeed) enemies[i].hullAngle += rotSpeed;
        else if (hullDiff < -rotSpeed) enemies[i].hullAngle -= rotSpeed;
        else enemies[i].hullAngle = targetGlobalAngle; 

        // 4. ROTAÇÃO DA TORRE
        if (seesPlayer && !isAvoiding) {
            float dx = playerX - enemies[i].x;
            float dz = playerZ - enemies[i].z;
            float pRad = atan2f(-dx, -dz); 
            float pDeg = pRad * (180.0f / 3.14159f);
            
            float tDiff = pDeg - (enemies[i].hullAngle + enemies[i].turretAngle);
            while (tDiff > 180) tDiff -= 360;
            while (tDiff < -180) tDiff += 360;
             
            if (tDiff > ENEMY_ROT_SPEED) enemies[i].turretAngle += ENEMY_ROT_SPEED;
            else if (tDiff < -ENEMY_ROT_SPEED) enemies[i].turretAngle -= ENEMY_ROT_SPEED;
        } else {
            enemies[i].turretAngle *= 0.90f;
        }

        // 5. MOVIMENTO
        // Só aplica movimento se a flag shouldMove for verdadeira
        if (shouldMove) {
            float rad = enemies[i].hullAngle * RADIAN_FACTOR;
            nextX -= sinf(rad) * currentMoveSpeed;
            nextZ -= cosf(rad) * currentMoveSpeed;

            float oldX = enemies[i].x;
            float oldZ = enemies[i].z;
            
            enemies[i].x = nextX;
            enemies[i].z = nextZ;
            
            if (wouldCollideEnemyTurret(i, enemies[i].turretAngle)) {
                enemies[i].x = oldX;
                enemies[i].z = oldZ;
                if (!seesPlayer) {
                    enemies[i].targetWanderAngle += 180.0f; 
                }
                enemies[i].hullAngle += 5.0f; 
            }
        }
    }
}