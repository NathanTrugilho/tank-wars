#ifndef PROJECTILE_H  
#define PROJECTILE_H

#include <math.h>
#include <time.h>
#include <enemy.h>
#include <tank.h>
#include <map.h>
#include <mybib.h>

#define MAX_BULLETS 100
#define BULLET_SPEED 1.0f
#define BULLET_SCALE_CORRECTION 0.15f
#define BULLET_SIDE_CORRECTION 0.07f

// Estrutura bullets
typedef struct {
    unsigned char active;
    float x, y, z;
    float horizontal_angle;
    float vertical_angle;
} Bullet;

extern Bullet player_bullet;

extern ObjModel shellModel;

// Atualiza bullets
void updateBullets();

// Carrega o obj da bullet e inicializa
void initBullet();

// Dispara bullet
void shootBullet();

// Desenha bullet
void drawBullet();

#endif