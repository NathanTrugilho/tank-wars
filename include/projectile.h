#ifndef PROJECTILE_H   // Se GAME_H não estiver definido
#define PROJECTILE_H

#include <math.h>
#include <time.h>

#define MAX_BULLETS 100

// Estrutura bullets
typedef struct {
    float x, z;
    float angle;
    int active;
} Bullet;

extern Bullet bullets[MAX_BULLETS];

// Atualiza bullets
void updateBullets();

// Inicializa bullets e inimigos
void initBulletsAndEnemies();
// Dispara bullet
void shootBullet();

#endif