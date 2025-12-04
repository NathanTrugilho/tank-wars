#include "enemy.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>       
#include <time.h>       
#include "collision.h" 
#include <projectile.h> 
#include "map.h"      
#include "tank.h" 

#ifndef RADIAN_FACTOR
#define RADIAN_FACTOR (3.14159265359 / 180.0)
#endif

Enemy enemies[MAX_ENEMIES];
ObjModel enemyHullModel;
ObjModel enemyTurretModel;
ObjModel enemyPipeModel;

#define SEPARATION_DIST 4.0f 

void initEnemies() {
    if (!loadOBJ("objects/hull.obj", "objects/hull.mtl", &enemyHullModel)) printf("Erro Hull\n");
    if (!loadOBJ("objects/turret.obj", "objects/turret.mtl", &enemyTurretModel)) printf("Erro Turret\n");
    if (!loadOBJ("objects/pipe.obj", "objects/pipe.mtl", &enemyPipeModel)) printf("Erro Pipe\n");

    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].x = (rand() % 50) - 25; 
        enemies[i].z = (rand() % 50) - 25;
        // Inicializa Y correto
        enemies[i].y = getTerrainHeight(enemies[i].x, enemies[i].z) + ENEMY_TANK_GROUND_DISTANCE;
        
        enemies[i].hullAngle = rand() % 360;   
        enemies[i].turretAngle = 0.0f;         
        enemies[i].wanderTimer = 0;
        enemies[i].targetWanderAngle = enemies[i].hullAngle;
        enemies[i].lastShootTime = 0; 
        enemies[i].stuckTimer = 0; // Inicializa zerado
    }
}

// Funções auxiliares no topo
void drawVisionCone(Enemy *e) {
    glPushAttrib(GL_ENABLE_BIT); 
    float totalAngle = e->hullAngle + e->turretAngle;
    float rad = totalAngle * RADIAN_FACTOR;
    float halfFovRad = (ENEMY_VIEW_ANGLE / 2.0f) * RADIAN_FACTOR;
    
    // Altura do olho (baseado no Y atual do inimigo)
    float y = e->y + 1.6f; // +1.6f pra ficar na altura do cano aprox
    
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
        glVertex3f(e->x, y, e->z); glVertex3f(e->x + cx, y, e->z + cz);
        glVertex3f(e->x, y, e->z); glVertex3f(e->x + lx, y, e->z + lz);
        glVertex3f(e->x, y, e->z); glVertex3f(e->x + rx, y, e->z + rz);
        glVertex3f(e->x + lx, y, e->z + lz); glVertex3f(e->x + rx, y, e->z + rz);
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

void drawEnemyTank(Enemy *e) {
    drawVisionCone(e);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glPushMatrix();
        // USA O Y DO INIMIGO
        glTranslatef(e->x, e->y, e->z); 
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
        if (enemies[i].alive) drawEnemyTank(&enemies[i]);
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void updateEnemies(float playerX, float playerZ) {
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;

        // Atualiza altura atual (caso ele tenha sido empurrado ou o mapa mude)
        enemies[i].y = getTerrainHeight(enemies[i].x, enemies[i].z) + ENEMY_TANK_GROUND_DISTANCE;

        // LÓGICA DE "Manobrar"
        // Se o inimigo estiver no estado "Stuck", ele ignora tudo e dá ré.
        if (enemies[i].stuckTimer > 0) {
            enemies[i].stuckTimer--;
            
            // Movimento de RÉ (Velocidade Negativa)
            float rad = enemies[i].hullAngle * RADIAN_FACTOR;
            float backSpeed = ENEMY_SPEED * 0.8f; // Ré um pouco mais lenta
            
            float nextX = enemies[i].x + sinf(rad) * backSpeed; // + sen/cos é ré (pois - é frente)
            float nextZ = enemies[i].z + cosf(rad) * backSpeed;
            float nextY = getTerrainHeight(nextX, nextZ) + ENEMY_TANK_GROUND_DISTANCE;

            // Verifica se a ré é segura (não tem parede atrás)
            Enemy futureBack = enemies[i];
            futureBack.x = nextX;
            futureBack.z = nextZ;
            futureBack.y = nextY;
            CollisionBox fh = makeEnemyHull(&futureBack);
            
            // Se a traseira não bater, aplica a ré
            if (!checkCollisionWithWorld(&fh)) {
                enemies[i].x = nextX;
                enemies[i].z = nextZ;
            }
            
            // Enquanto dá ré, tenta girar o corpo um pouco para mudar a direção de saída
            enemies[i].hullAngle += 2.0f; 
            
            continue; // Pula o resto da IA (não atira nem persegue enquanto manobra)
        }

        float nextX = enemies[i].x;
        float nextZ = enemies[i].z;
        float currentMoveSpeed = ENEMY_SPEED;
        float targetGlobalAngle = 0.0f; 
        int shouldMove = 1; 
        
        int seesPlayer = canSeePlayer(&enemies[i], playerX, playerZ);
        
        // DECISÃO
        if (seesPlayer) {
            if (now - enemies[i].lastShootTime > ENEMY_SHOOT_DELAY) {
                shootEnemyBullet(&enemies[i]);
                enemies[i].lastShootTime = now;
            }
            float dx = playerX - enemies[i].x;
            float dz = playerZ - enemies[i].z;
            float distSq = dx*dx + dz*dz;
            if (distSq < ENEMY_MIN_COMBAT_DIST * ENEMY_MIN_COMBAT_DIST) shouldMove = 0; 
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

        // REPULSÃO
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
                shouldMove = 1; 
                break; 
            }
        }

        // ROTAÇÃO HULL
        float hullDiff = targetGlobalAngle - enemies[i].hullAngle;
        while (hullDiff > 180) hullDiff -= 360;
        while (hullDiff < -180) hullDiff += 360;
        
        float nextHullAngle = enemies[i].hullAngle;
        float rotSpeed = isAvoiding ? ENEMY_ROT_SPEED * 2.0f : ENEMY_ROT_SPEED;

        if (hullDiff > rotSpeed) nextHullAngle += rotSpeed;
        else if (hullDiff < -rotSpeed) nextHullAngle -= rotSpeed;
        else nextHullAngle = targetGlobalAngle; 

        // Verifica colisão da rotação
        Enemy futureRotHull = enemies[i];
        futureRotHull.hullAngle = nextHullAngle;
        CollisionBox fh = makeEnemyHull(&futureRotHull);
        CollisionBox fp = makeEnemyPipe(&futureRotHull);

        if (!checkCollisionWithWorld(&fh) && !checkCollisionWithWorld(&fp)) {
            enemies[i].hullAngle = nextHullAngle;
        }

        // ROTAÇÃO TORRE
        if (seesPlayer && !isAvoiding) {
            float dx = playerX - enemies[i].x;
            float dz = playerZ - enemies[i].z;
            float pRad = atan2f(-dx, -dz); 
            float pDeg = pRad * (180.0f / 3.14159f);
            float tDiff = pDeg - (enemies[i].hullAngle + enemies[i].turretAngle);
            while (tDiff > 180) tDiff -= 360;
            while (tDiff < -180) tDiff += 360;
            
            float nextTurretAngle = enemies[i].turretAngle;
            if (tDiff > ENEMY_ROT_SPEED) nextTurretAngle += ENEMY_ROT_SPEED;
            else if (tDiff < -ENEMY_ROT_SPEED) nextTurretAngle -= ENEMY_ROT_SPEED;

            // Verifica colisão da rotação
            Enemy futureRot = enemies[i];
            futureRot.turretAngle = nextTurretAngle;
            CollisionBox ftr = makeEnemyTurret(&futureRot);
            CollisionBox fpr = makeEnemyPipe(&futureRot);

            if (!checkCollisionWithWorld(&fpr) && 
                !checkCollisionWithWorld(&ftr) &&
                !wouldCollideEnemyTurret(i, nextTurretAngle)) {
                 enemies[i].turretAngle = nextTurretAngle;
            }
        } else {
            enemies[i].turretAngle *= 0.90f;
        }

        // MOVIMENTO
        if (shouldMove) {
            float rad = enemies[i].hullAngle * RADIAN_FACTOR;
            
            // LÓGICA DE SENSOR (DISTÂNCIA SEGURA)
            float probeX = enemies[i].x - sinf(rad) * ENEMY_OBSTACLE_SAFE_DIST;
            float probeZ = enemies[i].z - cosf(rad) * ENEMY_OBSTACLE_SAFE_DIST;
            float probeY = getTerrainHeight(probeX, probeZ) + ENEMY_TANK_GROUND_DISTANCE;

            Enemy probeEnemy = enemies[i];
            probeEnemy.x = probeX;
            probeEnemy.z = probeZ;
            probeEnemy.y = probeY;

            CollisionBox probeBox = makeEnemyHull(&probeEnemy);
            
            // Se o sensor sentir a parede, ativa o MODO MANOBRAR
            if (checkCollisionWithWorld(&probeBox)) {
                shouldMove = 0;
                enemies[i].targetWanderAngle += 180.0f; // Vira para o outro lado
                enemies[i].stuckTimer = ENEMY_STUCK_TIME; // Ativa a ré por X frames
            }
            

            if (shouldMove) {
                nextX -= sinf(rad) * currentMoveSpeed;
                nextZ -= cosf(rad) * currentMoveSpeed;
                float nextY = getTerrainHeight(nextX, nextZ) + ENEMY_TANK_GROUND_DISTANCE; // ALTURA FUTURA

                float oldX = enemies[i].x;
                float oldZ = enemies[i].z;
                float oldY = enemies[i].y;

                Enemy futureEnemy = enemies[i];
                futureEnemy.x = nextX;
                futureEnemy.z = nextZ;
                futureEnemy.y = nextY; // Usa nova altura para checar colisão
                
                CollisionBox realFh = makeEnemyHull(&futureEnemy);
                CollisionBox realFt = makeEnemyTurret(&futureEnemy);
                CollisionBox realFp = makeEnemyPipe(&futureEnemy);
                
                // Hard Collision (Segurança final)
                int hitWorld = checkCollisionWithWorld(&realFh) || 
                               checkCollisionWithWorld(&realFt) || 
                               checkCollisionWithWorld(&realFp);

                if (hitWorld) {
                    // Se mesmo com o sensor ele bateu (ex: spawnou perto),
                    // Ativa o modo manobrar imediatamente.
                    enemies[i].x = oldX; 
                    enemies[i].z = oldZ;
                    enemies[i].y = oldY;
                    enemies[i].stuckTimer = ENEMY_STUCK_TIME; 
                } else {
                    enemies[i].x = nextX;
                    enemies[i].z = nextZ;
                    enemies[i].y = nextY;
                    
                    if (wouldCollideEnemyTurret(i, enemies[i].turretAngle)) {
                        enemies[i].x = oldX;
                        enemies[i].z = oldZ;
                        enemies[i].y = oldY;
                        if (!seesPlayer) enemies[i].targetWanderAngle += 180.0f; 
                        enemies[i].hullAngle += 5.0f; 
                    }
                }
            }
        }
    }
}