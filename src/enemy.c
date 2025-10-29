#include <enemy.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

Enemy enemies[MAX_ENEMIES];

void initEnemies() {
    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].hitbox.position.x = rand() % 50; // entre -18 e 18
        enemies[i].hitbox.position.y = 0.5;
        enemies[i].hitbox.position.z = rand() % 50; // entre -18 e 18
    }
}