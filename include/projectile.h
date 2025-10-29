#ifndef PROJECTILE_H   // Se GAME_H não estiver definido
#define PROJECTILE_H

#include <math.h>
#include <time.h>
#include <collision.h>

#define MAX_BULLETS 100

// Estrutura bullets
typedef struct {
    Box hitbox; //Talvez possa ser Vector3
    float angle;
    int active;
} Bullet;

extern Bullet bullets[MAX_BULLETS];

// Atualiza bullets
void updateBullets();

// Inicializa bullets e inimigos
void initBullets();
// Dispara bullet
void shootBullet();

#endif