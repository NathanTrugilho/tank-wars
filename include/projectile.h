#ifndef PROJECTILE_H  
#define PROJECTILE_H

#include <math.h>
#include <time.h>
// Forward declaration para evitar inclusão circular se necessário
#include <enemy.h> 
#include <tank.h>
#include <map.h>
#include <mybib.h>

#define MAX_BULLETS 100
#define MAX_ENEMY_BULLETS 50 // Limite de balas inimigas simultâneas

// Cadência de tiro do inimigo em milissegundos (1s)
#define ENEMY_SHOOT_DELAY 1000 

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
extern Bullet enemy_bullets[MAX_ENEMY_BULLETS]; // Array de balas inimigas

extern ObjModel shellModel;

// Atualiza bullets do jogador
void updateBullets();

// Gerenciamento de balas inimigas 
void initEnemyBullets();
void updateEnemyBullets();
void drawEnemyBullets();
void shootEnemyBullet(Enemy *e); // Inimigo 'e' atira

// Carrega o obj da bullet e inicializa
void initBullet();

// Dispara bullet do jogador
void shootBullet();

// Desenha bullet do jogador
void drawBullet();

#endif