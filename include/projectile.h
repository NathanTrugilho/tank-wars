#ifndef PROJECTILE_H   // Se GAME_H não estiver definido
#define PROJECTILE_H

#include <math.h>
#include <time.h>
#include <collision.h>

#define MAX_BULLETS 100

// Estrutura bullets
typedef struct {
    Vector3f position;
    float angle;
    int active;
    AABB hitbox;
} Bullet;

extern Bullet bullets[MAX_BULLETS];

// Atualiza bullets
void updateBullets();
// Inicializa bullets
void initBullets();
// Dispara bullet
void shootBullet();
// Desenha bullets
void drawBullets();

#endif