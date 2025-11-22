#ifndef ENEMY_H
#define ENEMY_H

#include "mybib.h" 

#define MAX_ENEMIES 30

// AJUSTE FINO
// WIDTH: Quanto menor, mais "fino" fica o colisor (evita bater girando)
// LENGTH: Quanto maior, mais longe ele alcança (corrige bater de frente)
// Esse é para as dimensões dos inimigos
// Base (Hull)
#define ENEMY_SCALE_HULL_W   0.90f
#define ENEMY_SCALE_HULL_L   0.90f

// Torre (Turret) - Notei que está bem pequeno (0.01), deve ser proposital
//#define ENEMY_SCALE_TURRET_W 0.01f
//#define ENEMY_SCALE_TURRET_L 0.01f

#define ENEMY_SCALE_TURRET_W 0.9f
#define ENEMY_SCALE_TURRET_L 0.9f

// Cano (Pipe)
#define ENEMY_SCALE_PIPE_W   0.3f 
#define ENEMY_SCALE_PIPE_L   1.0f 

// --- ALTURAS (Y-RANGE) DO INIMIGO ---
#define ENEMY_HULL_Y_MIN 0.6f
#define ENEMY_HULL_Y_MAX 1.5f

#define ENEMY_TURRET_Y_MIN 1.4f
#define ENEMY_TURRET_Y_MAX 1.9f

#define ENEMY_PIPE_Y_MIN   1.51f
#define ENEMY_PIPE_Y_MAX   1.7f

typedef struct {
    int alive;
    float x, z;
    float hullAngle; 
    float turretAngle; 
} Enemy;

extern Enemy enemies[MAX_ENEMIES];

extern ObjModel enemyHullModel;
extern ObjModel enemyTurretModel;
extern ObjModel enemyPipeModel;

void initEnemies();
void updateEnemies(float playerX, float playerZ); 
void drawEnemies();

#endif