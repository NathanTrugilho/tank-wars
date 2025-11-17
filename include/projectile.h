#ifndef PROJECTILE_H  
#define PROJECTILE_H

#include <math.h>
#include <time.h>

#define MAX_BULLETS 100

// Estrutura bullets
typedef struct {
    unsigned char active;
    float x, y, z;
    float horizontal_angle;
    float vertical_angle;
} Bullet;

extern Bullet player_bullet;

// Atualiza bullets
void updateBullets();

// Carrega o obj da bullet e inicializa
void initBullet();

// Dispara bullet
void shootBullet();

// Desenha bullet
void drawBullet();

#endif