#ifndef PROJECTILE_H  
#define PROJECTILE_H

#include <math.h>
#include <time.h>
#include <enemy.h>
#include <tank.h>
#include <map.h>
#include <mybib.h>

#define MAX_BULLETS 100
#define BULLET_SPEED 0.01f

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