#ifndef ENEMY_H
#define ENEMY_H

#include <tank.h>
#include <time.h>

#define MAX_ENEMIES 10

/* typedef struct {
    Tank enemyTank;
    // Heurísticas de movimento;
} Enemy; */

extern Tank enemies[MAX_ENEMIES];

void initEnemies();

void drawEnemies();

AABB getEnemyAABB(Tank *tank);

#endif
