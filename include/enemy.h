#ifndef ENEMY_H
#define ENEMY_H

#include <collision.h>

#define MAX_ENEMIES 10

// Estrutura inimigos
typedef struct Enemy{
    Box hitbox;
    int alive;
} Enemy;

extern Enemy enemies[MAX_ENEMIES];

void initEnemies();

#endif