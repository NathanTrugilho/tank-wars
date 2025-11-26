#ifndef ENEMY_H
#define ENEMY_H

#include "mybib.h" 

#define MAX_ENEMIES 2

// AJUSTE FINO
// WIDTH: Quanto menor, mais "fino" fica o colisor (evita bater girando)
// LENGTH: Quanto maior, mais longe ele alcança (corrige bater de frente)
// Esse é para as dimensões dos inimigos
// Base (Hull)
#define ENEMY_SCALE_HULL_W   0.90f
#define ENEMY_SCALE_HULL_L   0.90f

// Torre (Turret) 
#define ENEMY_SCALE_TURRET_W 0.9f
#define ENEMY_SCALE_TURRET_L 0.9f

// Cano (Pipe)
#define ENEMY_SCALE_PIPE_W   0.3f 
#define ENEMY_SCALE_PIPE_L   1.0f 

// ALTURAS (Y-RANGE) DO INIMIGO
#define ENEMY_HULL_Y_MIN 0.6f
#define ENEMY_HULL_Y_MAX 1.5f

#define ENEMY_TURRET_Y_MIN 1.4f
#define ENEMY_TURRET_Y_MAX 1.9f

#define ENEMY_PIPE_Y_MIN   1.51f
#define ENEMY_PIPE_Y_MAX   1.7f

// PARÂMETROS DE IA E VISÃO 
#define ENEMY_VIEW_RANGE 15.0f      // Distância máxima que o inimigo enxerga
#define ENEMY_VIEW_ANGLE 45.0f      // Ângulo de abertura do cone (em graus)
#define ENEMY_SPEED      0.05f      // Velocidade do inimigo (menor que a do player 0.1)
#define ENEMY_ROT_SPEED  1.5f       // Velocidade de rotação do inimigo
#define ENEMY_SHOOT_DELAY 1000 

// GRAU DE PREVISÃO/INTUIÇÃO (0.0 a 1.0)
// Isso auxilia na hora de escolher a direção de patrulha (wander).
// 0.0 = Totalmente aleatório
// 1.0 = Sempre mira diretamente para o jogador
#define ENEMY_TRACKING_BIAS 0.2f    

// DISTÂNCIA MÍNIMA DE COMBATE
// Se o inimigo estiver mais perto que isso, ele para de andar e só atira.
// Evita colisão física e bugs de "girar".
#define ENEMY_MIN_COMBAT_DIST 6.0f 

typedef struct {
    int alive;
    float x, z;
    float hullAngle; 
    float turretAngle; // O pipe gira junto com a torre
    
    // Controle de estado para patrulha (Wander)
    int wanderTimer;      // Tempo até mudar de direção aleatória
    float targetWanderAngle; // Ângulo alvo aleatório quando não vê o player
    // Controle de Tiro
    long lastShootTime; // Tempo do último disparo (em ms)
} Enemy;

extern Enemy enemies[MAX_ENEMIES];

extern ObjModel enemyHullModel;
extern ObjModel enemyTurretModel;
extern ObjModel enemyPipeModel;

void initEnemies();
void updateEnemies(float playerX, float playerZ); 
void drawEnemies();
#endif