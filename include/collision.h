#ifndef COLISION_H
#define COLISION_H

#include "mybib.h"

// --- AJUSTE FINO ---
// WIDTH: Quanto menor, mais "fino" fica o colisor (evita bater girando)
// LENGTH: Quanto maior, mais longe ele alcança (corrige bater de frente)

// Base (Hull): Quadrada, encolhe um pouco tudo
#define SCALE_HULL_W   0.75f
#define SCALE_HULL_L   0.90f

// Torre (Turret): Geralmente menor que a base
#define SCALE_TURRET_W 0.01f
#define SCALE_TURRET_L 0.01f

// Cano (Pipe): Precisa ser FINO (W=0.3) para não bater nos lados girando
// Mas precisa ser LONGO (L=0.95) para bater a ponta corretamente
#define SCALE_PIPE_W   0.5f 
#define SCALE_PIPE_L   0.70f 

// --- NOVO: AJUSTE DE ALTURA (Y-RANGE) ---
// Defina onde começa (MIN) e termina (MAX) cada parte no eixo Y do mundo.
// Exemplo: Se o Hull vai do chão (0.0) até quase 1 metro de altura.
#define HULL_Y_MIN 0.6f
#define HULL_Y_MAX 1.5f

// A torre fica em cima da base, então começa quase onde a base termina.
#define TURRET_Y_MIN 0.9f
#define TURRET_Y_MAX 1.6f

// O cano geralmente fica na mesma faixa de altura da torre.
// Importante: Se o PIPE_MIN > HULL_MAX, o cano NUNCA vai bater na base do inimigo!
#define PIPE_Y_MIN   1.51f
#define PIPE_Y_MAX   1.7f




typedef struct {
    float x, z;
} Point2D;

typedef struct {
    Point2D p[4];
} RotatedRect;

int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg);
int wouldCollideTurret(float nextTurretAngle);
int wouldCollideEnemyTurret(int enemyIndex, float nextTurretAngle);

void drawDebugHitbox(RotatedRect r, float yMin, float yMax); // Atualizei pra desenhar na altura certa
void debugDrawPlayerCollision();
void debugDrawEnemyCollision();

#endif