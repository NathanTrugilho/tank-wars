#include <projectile.h>
#include <enemy.h>
#include <tank.h>
#include <map.h>

Bullet bullets[MAX_BULLETS];

// Atualiza bullets
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].hitbox.position.x -= sinf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;
            bullets[i].hitbox.position.y = 0.3;
            bullets[i].hitbox.position.z -= cosf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;

            // Limites do mapa
            //if (bullets[i].hitbox.position.x < -MAP_SIZE/2 || bullets[i].hitbox.position.x > MAP_SIZE/2 || bullets[i].hitbox.position.z < -MAP_SIZE/2 || bullets[i].hitbox.position.z > MAP_SIZE/2)
                //bullets[i].active = 0;

            // Colisão com inimigos
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].alive) {
                    float dx = bullets[i].hitbox.position.x - enemies[j].hitbox.position.x;
                    float dz = bullets[i].hitbox.position.z - enemies[j].hitbox.position.z;
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

// Inicializa bullets
void initBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = 0;
}

// Dispara bullet
void shootBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].hitbox.position.x = playerTank.hitbox.position.x;
            bullets[i].hitbox.position.z = playerTank.hitbox.position.z;
            bullets[i].angle = playerTank.tankAngle;
            break;
        }
    }
}