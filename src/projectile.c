#include <projectile.h>

Bullet player_bullet;
ObjModel shellModel;

// Atualiza bullets
void updateBullets() {
    if (!player_bullet.active) return;

    float vector_x = -sinf(player_bullet.horizontal_angle * RADIAN_FACTOR);
    float vector_z = -cosf(player_bullet.horizontal_angle * RADIAN_FACTOR);
    float vector_y =  sinf(player_bullet.vertical_angle   * RADIAN_FACTOR);

    player_bullet.x += vector_x * BULLET_SPEED;
    player_bullet.z += vector_z * BULLET_SPEED;
    player_bullet.y += vector_y * BULLET_SPEED;

    /*
    if (player_bullet.x < -MAP_SIZE/2 || player_bullet.x > MAP_SIZE/2 ||
        player_bullet.z < -MAP_SIZE/2 || player_bullet.z > MAP_SIZE/2 ||
        player_bullet.y < 0)
        player_bullet.active = FALSE; */

    for (int j = 0; j < MAX_ENEMIES; j++) {
        if (!enemies[j].alive) continue;

        float dx = player_bullet.x - enemies[j].x;
        float dz = player_bullet.z - enemies[j].z;
        float dist = sqrtf(dx*dx + dz*dz);

        if (dist < 0.7f) {
            enemies[j].alive = FALSE;
            player_bullet.active = FALSE;
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
    static unsigned long lastShootTime = 0; // Só é zero quando inicializar
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);;

    // Serve pra botar um cooldown antes de atirar dnv
    if (now - lastShootTime < 3000) return;
    lastShootTime = now;

    player_bullet.active = TRUE;
    player_bullet.x = tankX;
    player_bullet.y = 1;
    player_bullet.z = tankZ;

    player_bullet.horizontal_angle = turretAngle; // inclinação horizontal
    player_bullet.vertical_angle = pipeAngle; // inclinação vertical
}

void drawBullet(){
    if(!player_bullet.active) return;
    
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(player_bullet.x, player_bullet.y, player_bullet.z);

        glRotatef(player_bullet.horizontal_angle, 0.0f, 1.0f, 0.0f);
        glRotatef(player_bullet.vertical_angle, 1.0f, 0.0f, 0.0f);

        drawModel(&shellModel); 
        //drawBox(hullModel.box); // Hitbox da base

    glPopMatrix();
    glDisable(GL_TEXTURE_2D); // Tem que desativar se não fica tudo escuro pq as outras coisas n têm textura
}