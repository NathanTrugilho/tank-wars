#include <projectile.h>
#include <enemy.h>
#include <tank.h>
#include <map.h>

Bullet bullets[MAX_BULLETS];

// Atualiza bullets
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x -= sinf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;
            bullets[i].z -= cosf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;

            // Limites do mapa
            //if (bullets[i].x < -MAP_SIZE/2 || bullets[i].x > MAP_SIZE/2 || bullets[i].z < -MAP_SIZE/2 || bullets[i].z > MAP_SIZE/2)
                //bullets[i].active = 0;

            // Colisão com inimigos
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].alive) {
                    float dx = bullets[i].x - enemies[j].x;
                    float dz = bullets[i].z - enemies[j].z;
                    float dist = sqrtf(dx*dx + dz*dz);
                    if (dist < 0.7f) {
                        enemies[j].alive = 0;
                        bullets[i].active = 0;
                    }
                }
            }
        }
    }
}

// Inicializa bullets e inimigos
void initBulletsAndEnemies() {
    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = 0;

    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].x = rand() % 50; // entre -18 e 18
        enemies[i].z = rand() % 50; // entre -18 e 18
    }
}

// Dispara bullet
void shootBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].x = tankX;
            bullets[i].z = tankZ;
            bullets[i].angle = turretAngle;
            break;
        }
    }
}