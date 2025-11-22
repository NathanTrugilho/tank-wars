// enemy.c
#include "enemy.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>       // Para rand()
#include <time.h>       // Para time()
#include <collision.h>
Enemy enemies[MAX_ENEMIES];

// Modelos 3D compartilhados por todos os inimigos
// Meu objetivo é criar uma estrutura geral que funcione para todos os inimigos, player, mapa, construções etc.
ObjModel enemyHullModel;
ObjModel enemyTurretModel;
ObjModel enemyPipeModel;

void initEnemies() {
    // Carrega os modelos dos inimigos (usando os mesmos do player por enquanto)
    if (!loadOBJ("objects/hull.obj", "objects/hull.mtl", &enemyHullModel)) {
        printf("Erro ao carregar Base do inimigo.\n");
    }
    if (!loadOBJ("objects/turret.obj", "objects/turret.mtl", &enemyTurretModel)) {
        printf("Erro ao carregar Torre do inimigo.\n");
    }
    if (!loadOBJ("objects/pipe.obj", "objects/pipe.mtl", &enemyPipeModel)) {
        printf("Erro ao carregar Canhão do inimigo.\n");
    }

    // 2. Inicializa as posições
    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].x = (rand() % 50) - 25; // Exemplo: posição aleatória entre -25 e 25
        enemies[i].z = (rand() % 50) - 25;
        enemies[i].hullAngle = rand() % 360;   // Rotação inicial aleatória
        enemies[i].turretAngle = 0.0f;         // Torre começa alinhada
    }
}

// Função interna para desenhar UM inimigo específico
void drawSingleEnemy(Enemy *e) {
    glPushMatrix();
        // Move para a posição DESTE inimigo
        glTranslatef(e->x, 0.5f, e->z); // 0.5f é a altura do chão (tankY)

        // Desenha a Base
        glPushMatrix();
            glRotatef(e->hullAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&enemyHullModel); 
            //drawBox(enemyHullModel.box); essa função é a "hitbox" só que feita pelo mybib. Logo é só desconsiderar
        glPopMatrix();

        // Desenha a Torre e o Canhão
        glPushMatrix();
            // ATENÇÃO:
            // A torre gira em relação à base OU ao mundo? 
            // Se for em relação ao mundo, usa só turretAngle.
            // Se for em relação à base, some (hullAngle + turretAngle).
            glRotatef(e->hullAngle + e->turretAngle, 0.0f, 1.0f, 0.0f);
            
            drawModel(&enemyTurretModel);
            //drawBox(enemyTurretModel.box); // Opcional

            // O canhão não precisa girar pra cima/baixo por enquanto para inimigos
            drawModel(&enemyPipeModel);
            //drawBox(enemyPipeModel.box); // Opcional
        glPopMatrix();

    glPopMatrix();
}

// Função principal chamada pelo game loop para desenhar TODOS
void drawEnemies() {
    glEnable(GL_TEXTURE_2D); // Garante texturas ligadas para os modelos
    glPushMatrix();
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
             // Mudar a cor para diferenciar do player se usar o mesmo modelo
             glColor3f(1.0f, 0.5f, 0.5f); // Tom avermelhado (precisa que o drawModel suporte glColor misturado com textura)
             drawSingleEnemy(&enemies[i]);
        }
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

// Se quiser que eles façam algo, atualize aqui
void updateEnemies(float playerX, float playerZ) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
            
            // Lógica simples: Tentar girar 1 grau
            float currentAngle = enemies[i].turretAngle;
            float nextAngle = currentAngle + 1.0f;
            if (nextAngle > 360.0f) nextAngle -= 360.0f;

            // --- VERIFICAÇÃO DE COLISÃO ANTES DE GIRAR ---
            // Se girar vai bater no player, NÃO GIRE.
            if (!wouldCollideEnemyTurret(i, nextAngle)) {
                enemies[i].turretAngle = nextAngle;
            } else {
                // Opcional: Se colidiu, pode tentar girar para o outro lado
                // ou simplesmente ficar parado esperando o player sair da frente.
                // printf("Inimigo %d travado pelo canhão!\n", i);
            }
        }
    }
}