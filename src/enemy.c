#include <enemy.h>

Tank enemies[MAX_ENEMIES];

void initEnemies() {
    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].position.x = rand() % 50; // entre -18 e 18
        enemies[i].position.y = 0.5;
        enemies[i].position.z = rand() % 50; // entre -18 e 18
        // Inicializa a AABB do inimigo
        enemies[i].hitbox = getEnemyAABB(&enemies[i]);
    }
}

void drawEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
            glPushMatrix();
                glTranslatef(enemies[i].position.x, enemies[i].position.y, enemies[i].position.z);
                glColor3f(0.1f, 0.1f, 0.7f);
                glScalef(1.0f, 0.5f, 2.0f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
}

AABB getEnemyAABB(Tank *tank) {
    AABB box;
    // O tamanho do tanque é 1.0 (X) x 0.5 (Y) x 2.0 (Z) na escala local.
    // A AABB é sempre alinhada aos eixos, ignorando a rotação.
    box.min.x = tank->position.x - 0.5f;
    box.max.x = tank->position.x + 0.5f;
    
    box.min.y = tank->position.y - 0.25f; 
    box.max.y = tank->position.y + 0.25f;

    box.min.z = tank->position.z - 1.0f;
    box.max.z = tank->position.z + 1.0f;
    
    return box;
}