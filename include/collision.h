#ifndef COLISION_H
#define COLISION_H

#include "mybib.h"

// --- AJUSTE FINO ---
// WIDTH: Quanto menor, mais "fino" fica o colisor (evita bater girando)
// LENGTH: Quanto maior, mais longe ele alcança (corrige bater de frente)

// Base (Hull): Quadrada, encolhe um pouco tudo
#define SCALE_HULL_W   0.75f
#define SCALE_HULL_L   0.75f

// Torre (Turret): Geralmente menor que a base
#define SCALE_TURRET_W 0.60f
#define SCALE_TURRET_L 0.60f

// Cano (Pipe): Precisa ser FINO (W=0.3) para não bater nos lados girando
// Mas precisa ser LONGO (L=0.95) para bater a ponta corretamente
#define SCALE_PIPE_W   0.30f 
#define SCALE_PIPE_L   0.98f 

typedef struct {
    float x, z;
} Point2D;

typedef struct {
    Point2D p[4];
} RotatedRect;

int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg);
int wouldCollideTurret(float nextTurretAngle);
int wouldCollideEnemyTurret(int enemyIndex, float nextTurretAngle);

// Função NOVA para desenhar as caixas de colisão (chame no display() para testar)
void drawDebugHitbox(RotatedRect r); 
// Função auxiliar para desenhar todas as caixas do player (para facilitar)
void debugDrawPlayerCollision();

#endif