#include <projectile.h>
#include <collision.h> 
#include <enemy.h>     
#include <game.h>      
#include <tank.h> // Necessário para acessar tankX, tankZ, hullModel etc.

Bullet player_bullet;
Bullet enemy_bullets[MAX_ENEMY_BULLETS]; // Pool de balas inimigas

ObjModel shellModel;

// Altura aproximada da hitbox da bala
#define BULLET_BOX_HEIGHT 0.15f 

#ifndef RADIAN_FACTOR
#define RADIAN_FACTOR (3.14159265359 / 180.0)
#endif

// --- FUNÇÕES DO JOGADOR (MANTIDAS IGUAIS) ---

void updateBullets() {
    if (!player_bullet.active) return;

    float horizontal_without_vertical = player_bullet.horizontal_angle * RADIAN_FACTOR;
    float horizontal_scale = cosf(player_bullet.vertical_angle * RADIAN_FACTOR); 

    float vector_x = -sinf(horizontal_without_vertical) * horizontal_scale;
    float vector_z = -cosf(horizontal_without_vertical) * horizontal_scale;
    float vector_y =  sinf(player_bullet.vertical_angle   * RADIAN_FACTOR);

    player_bullet.x += vector_x * BULLET_SPEED;
    player_bullet.z += vector_z * BULLET_SPEED;
    player_bullet.y += vector_y * BULLET_SPEED;

    // Colisão Bala Jogador vs Inimigos
    float bMinY = player_bullet.y - (BULLET_BOX_HEIGHT / 2.0f);
    float bMaxY = player_bullet.y + (BULLET_BOX_HEIGHT / 2.0f);

    CollisionBox bulletBox = getCollisionBox(
        &shellModel.box,
        player_bullet.x, player_bullet.y, player_bullet.z,
        player_bullet.horizontal_angle, player_bullet.vertical_angle, 
        BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION,             
        bMinY, bMaxY                                                  
    );

    for (int j = 0; j < MAX_ENEMIES; j++) {
        if (!enemies[j].alive) continue;

        float ex = enemies[j].x;
        float ez = enemies[j].z;
        float eHullA = enemies[j].hullAngle;
        float eTurretA = enemies[j].turretAngle; 
        float eGlobA = eHullA + eTurretA;        

        // Recria caixas do inimigo para checagem
        // Nota: Assumindo tankY para inimigos também (chão plano)
        CollisionBox eHull = getCollisionBox(
            &enemyHullModel.box, ex, tankY, ez, eHullA, 0.0f, 
            ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L, HULL_Y_MIN, HULL_Y_MAX
        );
        CollisionBox eTurret = getCollisionBox(
            &enemyTurretModel.box, ex, tankY, ez, eGlobA, 0.0f,
            ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX
        );
        CollisionBox ePipe = getCollisionBox(
            &enemyPipeModel.box, ex, tankY, ez, eGlobA, 0.0f,
            ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX
        );

        if (checkCollisionOBB(&bulletBox, &eHull) || 
            checkCollisionOBB(&bulletBox, &eTurret) || 
            checkCollisionOBB(&bulletBox, &ePipe)) {
            enemies[j].alive = 0;     
            player_bullet.active = 0; 
            break; 
        }
    }
}

// --- NOVO: SISTEMA DE BALAS INIMIGAS ---

void initEnemyBullets() {
    for(int i=0; i<MAX_ENEMY_BULLETS; i++) {
        enemy_bullets[i].active = 0;
    }
}

void shootEnemyBullet(Enemy *e) {
    // Procura um slot livre
    int slot = -1;
    for(int i=0; i<MAX_ENEMY_BULLETS; i++) {
        if(!enemy_bullets[i].active) {
            slot = i;
            break;
        }
    }
    if (slot == -1) return; // Limite atingido

    Bullet *b = &enemy_bullets[slot];
    b->active = 1;
    
    // Configura ângulos
    // A bala sai alinhada com a torre do inimigo
    float totalAngle = e->hullAngle + e->turretAngle;
    b->horizontal_angle = totalAngle;
    b->vertical_angle = 0.0f; // Inimigo por enquanto não tem pitch

    // Calcula posição inicial (Ponta do cano do inimigo)
    float rad = totalAngle * RADIAN_FACTOR;
    
    // Comprimento do cano inimigo (ajuste se necessário, usando PIPE_LENGTH do player como base)
    float pipeLen = PIPE_LENGTH; // Ou use uma constante específica se tiver
    float pipeH = (ENEMY_PIPE_Y_MIN + ENEMY_PIPE_Y_MAX) / 2.0f;

    // Vetor forward é -sin, -cos
    b->x = e->x - sinf(rad) * pipeLen;
    b->z = e->z - cosf(rad) * pipeLen;
    b->y = pipeH; 
}

void updateEnemyBullets() {
    // Gera as caixas de colisão do PLAYER uma única vez por frame
    // para testar contra todas as balas
    CollisionBox pHull = getCollisionBox(
        &hullModel.box, tankX, tankY, tankZ, hullAngle, 0.0f, 
        SCALE_HULL_W, SCALE_HULL_L, HULL_Y_MIN, HULL_Y_MAX
    );
    CollisionBox pTurret = getCollisionBox(
        &turretModel.box, tankX, tankY, tankZ, turretAngle, 0.0f, 
        SCALE_TURRET_W, SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX
    );
    CollisionBox pPipe = getCollisionBox(
        &pipeModel.box, tankX, tankY, tankZ, turretAngle, pipeAngle, 
        SCALE_PIPE_W, SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX
    );

    for(int i=0; i<MAX_ENEMY_BULLETS; i++) {
        if(!enemy_bullets[i].active) continue;

        Bullet *b = &enemy_bullets[i];

        // Movimento
        float h_rad = b->horizontal_angle * RADIAN_FACTOR;
        float v_rad = b->vertical_angle * RADIAN_FACTOR;
        float h_scale = cosf(v_rad);

        b->x += -sinf(h_rad) * h_scale * BULLET_SPEED;
        b->z += -cosf(h_rad) * h_scale * BULLET_SPEED;
        b->y +=  sinf(v_rad) * BULLET_SPEED;

        // Limite de mapa (Opcional, para não voar pra sempre)
        if(fabs(b->x) > 100 || fabs(b->z) > 100 || b->y < 0) {
            b->active = 0;
            continue;
        }

        // --- COLISÃO COM O JOGADOR ---
        float bMinY = b->y - (BULLET_BOX_HEIGHT / 2.0f);
        float bMaxY = b->y + (BULLET_BOX_HEIGHT / 2.0f);

        CollisionBox bBox = getCollisionBox(
            &shellModel.box, b->x, b->y, b->z,
            b->horizontal_angle, b->vertical_angle,
            BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION,
            bMinY, bMaxY
        );

        if (checkCollisionOBB(&bBox, &pHull) || 
            checkCollisionOBB(&bBox, &pTurret) || 
            checkCollisionOBB(&bBox, &pPipe)) {
            
            // Acertou o jogador!
            b->active = 0; // Bala some
            // Aqui você colocaria: playerHealth--;
            // printf("Player atingido!\n");
        }
    }
}

void drawEnemyBullets() {
    glEnable(GL_TEXTURE_2D);
    for(int i=0; i<MAX_ENEMY_BULLETS; i++) {
        if(!enemy_bullets[i].active) continue;
        
        Bullet *b = &enemy_bullets[i];
        glPushMatrix();
            glTranslatef(b->x, b->y, b->z);
            glScalef(BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION);
            glRotatef(b->horizontal_angle, 0.0f, 1.0f, 0.0f);
            glRotatef(b->vertical_angle, 1.0f, 0.0f, 0.0f);
            drawModel(&shellModel); 
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}

// --- INICIALIZAÇÃO GERAL ---

void initBullet() {
    if (loadOBJ("objects/shell.obj", "objects/shell.mtl", &shellModel)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo do projetil.\n");
    } 
    player_bullet.active = 0;
    initEnemyBullets(); // Inicializa o array de inimigos também
}

void shootBullet() {
    static unsigned long lastShootTime = 0; 
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);;

    if (now - lastShootTime < 3000) return;
    lastShootTime = now;

    player_bullet.active = 1;
    player_bullet.horizontal_angle = turretAngle; 
    player_bullet.vertical_angle = pipeAngle; 
    
    float h_rad = turretAngle * RADIAN_FACTOR;
    float v_rad = pipeAngle * RADIAN_FACTOR;

    float horizontal_dist = cosf(v_rad) * PIPE_LENGTH;
    float right_x = cosf(h_rad);
    float right_z = -sinf(h_rad);

    player_bullet.x = tankX - sinf(h_rad) * horizontal_dist - (right_x * BULLET_SIDE_CORRECTION);
    player_bullet.z = tankZ - cosf(h_rad) * horizontal_dist - (right_z * BULLET_SIDE_CORRECTION);
    player_bullet.y = (tankY + PIPE_HEIGHT) + sinf(v_rad) * PIPE_LENGTH;
}

void drawBullet(){
    if(!player_bullet.active) return;
    
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(player_bullet.x, player_bullet.y, player_bullet.z);
        glScalef(BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION);
        glRotatef(player_bullet.horizontal_angle, 0.0f, 1.0f, 0.0f);
        glRotatef(player_bullet.vertical_angle, 1.0f, 0.0f, 0.0f);
        drawModel(&shellModel); 
    glPopMatrix();
    glDisable(GL_TEXTURE_2D); 
}