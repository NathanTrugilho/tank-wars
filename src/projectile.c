#include <projectile.h>
#include <collision.h> // Agora traz CollisionBox e checkCollisionOBB
#include <enemy.h>     // Para acessar os modelos e constantes dos inimigos
#include <game.h>      // Para acessar tankY se necessario (ou variaveis globais)

Bullet player_bullet;
ObjModel shellModel;

// Altura aproximada da hitbox da bala (pequena caixa)
#define BULLET_BOX_HEIGHT 0.15f 

// Se RADIAN_FACTOR não estiver definido aqui ou em collision.h
#ifndef RADIAN_FACTOR
#define RADIAN_FACTOR (3.14159265359 / 180.0)
#endif

// Atualiza bullets
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

    // --- NOVA LÓGICA DE COLISÃO OBB (3D) ---

    // 1. Criar a CollisionBox da Bala
    // Como a bala voa livremente (Y varia), definimos o Y_MIN e Y_MAX 
    // relativos à posição atual dela para criar uma caixinha ao redor do centro.
    float bMinY = player_bullet.y - (BULLET_BOX_HEIGHT / 2.0f);
    float bMaxY = player_bullet.y + (BULLET_BOX_HEIGHT / 2.0f);

    CollisionBox bulletBox = getCollisionBox(
        &shellModel.box,
        player_bullet.x, player_bullet.y, player_bullet.z,
        player_bullet.horizontal_angle, player_bullet.vertical_angle, // Yaw e Pitch
        BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION,             // Escala W e L
        bMinY, bMaxY                                                  // Altura dinâmica
    );

    // 2. Verificar colisão contra TODOS os inimigos
    for (int j = 0; j < MAX_ENEMIES; j++) {
        if (!enemies[j].alive) continue;

        float ex = enemies[j].x;
        float ez = enemies[j].z;
        float eHullA = enemies[j].hullAngle;
        float eTurretA = enemies[j].turretAngle; // Relativo ao Hull
        float eGlobA = eHullA + eTurretA;        // Angulo global da torre/cano

        // --- Recriar as CollisionBoxes do Inimigo ---
        // Usamos as mesmas constantes definidas em collision.h e enemy.h
        // Nota: tankY é uma variável global definida em tank.h, assumimos que o inimigo está no mesmo chão
        
        // HULL (Base)
        CollisionBox eHull = getCollisionBox(
            &enemyHullModel.box, 
            ex, tankY, ez, 
            eHullA, 0.0f, // Pitch 0
            ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L,
            HULL_Y_MIN, HULL_Y_MAX
        );

        // TURRET (Torre)
        CollisionBox eTurret = getCollisionBox(
            &enemyTurretModel.box,
            ex, tankY, ez,
            eGlobA, 0.0f,
            ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L,
            TURRET_Y_MIN, TURRET_Y_MAX
        );

        // PIPE (Cano) - Inimigo Pitch = 0 por enquanto
        CollisionBox ePipe = getCollisionBox(
            &enemyPipeModel.box,
            ex, tankY, ez,
            eGlobA, 0.0f,
            ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L,
            PIPE_Y_MIN, PIPE_Y_MAX
        );

        // --- Verificar Colisão (OBB vs OBB) ---
        int hit = 0;

        if (checkCollisionOBB(&bulletBox, &eHull)) {
            hit = 1;
        }
        else if (checkCollisionOBB(&bulletBox, &eTurret)) {
            hit = 1;
        }
        else if (checkCollisionOBB(&bulletBox, &ePipe)) {
            hit = 1;
        }

        if (hit) {
            enemies[j].alive = FALSE;     // Mata inimigo
            player_bullet.active = FALSE; // Destroi bala
            break; 
        }
    }
}

// Carrega o modelo da bullet
void initBullet() {
    if (loadOBJ("objects/shell.obj", "objects/shell.mtl", &shellModel)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo do projetil.\n");
    } 
    player_bullet.active = FALSE;
}

// Dispara bullet
void shootBullet() {
    static unsigned long lastShootTime = 0; 
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);;

    if (now - lastShootTime < 3000) return;
    lastShootTime = now;

    player_bullet.active = TRUE;

    player_bullet.horizontal_angle = turretAngle; 
    player_bullet.vertical_angle = pipeAngle; 
    
    float h_rad = turretAngle * RADIAN_FACTOR;
    float v_rad = pipeAngle * RADIAN_FACTOR;

    // Calcula posição inicial na ponta do cano
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
        
        // Se quiser debug visual da caixa da bala:
        // Mas precisa reconstruir a caixa aqui ou calcular de novo
        // drawBox(shellModel.box); 

    glPopMatrix();
    glDisable(GL_TEXTURE_2D); 
}