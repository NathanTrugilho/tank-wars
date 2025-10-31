#include <projectile.h>
#include <enemy.h>
#include <tank.h>
#include <map.h>

Bullet bullets[MAX_BULLETS];

// Atualiza bullets
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.x -= sinf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;
            bullets[i].position.z -= cosf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;

            // Limites do mapa
            //if (bullets[i].x < -MAP_SIZE/2 || bullets[i].x > MAP_SIZE/2 || bullets[i].z < -MAP_SIZE/2 || bullets[i].z > MAP_SIZE/2)
                //bullets[i].active = 0;

            // Colisão com inimigos
            /*for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].alive) {
                    float dx = bullets[i].x - enemies[j].x;
                    float dz = bullets[i].z - enemies[j].z;
                    float dist = sqrtf(dx*dx + dz*dz);
                    if (dist < 0.7f) {
                        //enemies[j].alive = 0;
                        bullets[i].active = 0;
                    }
                }
            }*/
        }
    }
}

// Inicializa bullets
void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = 0;
}

// Dispara bullet
void shootBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].position.x = player.position.x;
            bullets[i].position.y = 0.3;
            bullets[i].position.z = player.position.z;
            bullets[i].angle = player.tankAngle;
            break;
        }
    }
}

void drawBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            glPushMatrix();
                glTranslatef(bullets[i].position.x, bullets[i].position.y, bullets[i].position.z);
                glColor3f(1.0f, 1.0f, 0.0f);
                glutSolidSphere(0.2, 10, 10);
            glPopMatrix();
        }
    }
}