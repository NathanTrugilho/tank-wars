#ifndef COLISION_H
#define COLISION_H

#include "mybib.h"
#include <power_up.h>

// Forward Declaration para evitar loop de include
struct Enemy; 

// WIDTH: Quanto menor, mais "fino" fica o colisor (evita bater girando)
// LENGTH: Quanto maior, mais longe ele alcança (corrige bater de frente)

// Base (Hull): Quadrada, encolhe um pouco tudo
#define SCALE_HULL_W   0.90f
#define SCALE_HULL_L   0.90f

// Torre (Turret): Geralmente menor que a base
#define SCALE_TURRET_W 0.90f
#define SCALE_TURRET_L 0.90f

// Cano (Pipe): Precisa ser FINO para não bater nos lados girando
// Mas precisa ser LONGO (L=1.0) para bater a ponta corretamente
#define SCALE_PIPE_W   0.3f 
#define SCALE_PIPE_L   1.0f 

// AJUSTE DE ALTURA (Y-RANGE)
// Define onde começa (MIN) e termina (MAX) cada parte no eixo Y do mundo.
#define HULL_Y_MIN 0.6f
#define HULL_Y_MAX 1.5f

// A torre fica em cima da base, então começa quase onde a base termina.
#define TURRET_Y_MIN 1.4f
#define TURRET_Y_MAX 1.9f

// O cano geralmente fica na mesma faixa de altura da torre.
// Importante: Se o PIPE_MIN > HULL_MAX, o cano NUNCA vai bater na base do inimigo!
#define PIPE_Y_MIN   1.51f
#define PIPE_Y_MAX   1.7f

typedef struct {
    float x, y, z;
} Point3D;

typedef struct {
    Point3D center;      // Centro da caixa no mundo
    Point3D axis[3];     // Vetores de direção (X, Y, Z rotacionados)
    float halfSize[3];   // Metade da largura, altura e profundidade
    Point3D corners[8];  // Os 8 cantos para desenhar o debug
} CollisionBox;

// Função principal que gera a caixa 
CollisionBox getCollisionBox(const Box *localBox, float tx, float ty, float tz, 
                             float angleYaw, float anglePitch, 
                             float scaleW, float scaleL, 
                             float yMinFixed, float yMaxFixed); 

int checkCollisionOBB(CollisionBox *a, CollisionBox *b);

// Verifica colisão de uma caixa dinâmica contra todos os objetos estáticos do mapa
int checkCollisionWithWorld(CollisionBox *dynamicBox);

// Verifica se existe uma linha de visão livre entre dois pontos (Raycast)
// Com isso se o jogador se esconder atrás de um objeto o inimigo perde a visão dele
// Retorna 1 se estiver livre, 0 se houver um prédio no caminho.
int checkLineOfSight(Point3D start, Point3D end);

// Funções do Jogo
int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg);
int wouldCollideTurret(float nextTurretAngle);
int wouldCollideEnemyTurret(int enemyIndex, float nextTurretAngle);

// Debug Visual
void drawDebugBox(CollisionBox b); 
void debugDrawPlayerCollision();
void debugDrawEnemyCollision();
void debugDrawWorldCollisions();

// (HELPERS)

// Player (Usa variáveis globais do tank.c)
CollisionBox makePlayerHull(float x, float z, float angle);
CollisionBox makePlayerTurret(float x, float z, float angle);
CollisionBox makePlayerPipe(float x, float z, float tAngle, float pAngle);

// Enemy (Usa a struct Enemy)
CollisionBox makeEnemyHull(struct Enemy *e);
CollisionBox makeEnemyTurret(struct Enemy *e);
CollisionBox makeEnemyPipe(struct Enemy *e);

int checkTankPowerUpCollision(PowerUpInstance *p);
int checkAllPowerUpCollisions(PowerUpInstance powerUps[]);

#endif