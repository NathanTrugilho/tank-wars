#include "collision.h"
#include "tank.h"
#include "enemy.h"
#include <math.h>
#include <float.h>
#include <stdio.h>

#define RADIAN_FACTOR (3.14159265359 / 180.0)

// --- FUNÇÕES MATEMÁTICAS AUXILIARES (Implementação do zero) ---

// Rotaciona um ponto (x, z) em torno da origem (0,0)
Point2D rotatePoint(float x, float z, float angleRad) {
    Point2D res;
    float c = cos(angleRad);
    float s = sin(angleRad);
    res.x = x * c - z * s;
    res.z = x * s + z * c;
    return res;
}

// Pega a Box local do modelo e transforma em um Retângulo rotacionado no mundo
RotatedRect getRotatedRect(const Box *localBox, float tx, float tz, float angleDeg) {
    RotatedRect rect;
    float angleRad = angleDeg * RADIAN_FACTOR;

    // Os 4 cantos da caixa original (ignorando Y, pois é top-down)
    // Ordem: Inferior-Esq, Superior-Esq, Superior-Dir, Inferior-Dir
    float cornersX[4] = { localBox->minX, localBox->minX, localBox->maxX, localBox->maxX };
    float cornersZ[4] = { localBox->minZ, localBox->maxZ, localBox->maxZ, localBox->minZ };

    for (int i = 0; i < 4; i++) {
        // 1. Rotaciona o ponto local
        Point2D p = rotatePoint(cornersX[i], cornersZ[i], angleRad);
        // 2. Translada para a posição do mundo
        rect.p[i].x = p.x + tx;
        rect.p[i].z = p.z + tz;
    }
    return rect;
}

// --- IMPLEMENTAÇÃO DO SAT (Separating Axis Theorem) ---
// Verifica se dois retângulos convexos colidem.
// Se houver QUALQUER eixo onde as sombras não se tocam, não há colisão.

int checkSATCollision(RotatedRect r1, RotatedRect r2) {
    // Precisamos testar os eixos perpendiculares às arestas de ambos os retângulos.
    // Como são retângulos, testamos apenas 2 eixos de cada (4 no total).
    
    RotatedRect *rects[2] = { &r1, &r2 };

    for (int i = 0; i < 2; i++) {
        RotatedRect *curr = rects[i];
        
        // Para cada aresta do retângulo atual (só precisamos das 2 primeiras pois as outras são paralelas)
        for (int j = 0; j < 2; j++) {
            // Pega dois vértices para formar a aresta
            Point2D p1 = curr->p[j];
            Point2D p2 = curr->p[(j + 1) % 4];

            // Vetor da aresta
            float edgeX = p2.x - p1.x;
            float edgeZ = p2.z - p1.z;

            // Normal (perpendicular) ao eixo (-z, x)
            float axisX = -edgeZ;
            float axisZ = edgeX;

            // Normaliza o eixo (opcional, mas bom para consistência)
            float len = sqrt(axisX * axisX + axisZ * axisZ);
            if (len == 0) continue;
            axisX /= len;
            axisZ /= len;

            // Projeta o retângulo A no eixo
            float minA = FLT_MAX, maxA = -FLT_MAX;
            for (int k = 0; k < 4; k++) {
                float proj = r1.p[k].x * axisX + r1.p[k].z * axisZ;
                if (proj < minA) minA = proj;
                if (proj > maxA) maxA = proj;
            }

            // Projeta o retângulo B no eixo
            float minB = FLT_MAX, maxB = -FLT_MAX;
            for (int k = 0; k < 4; k++) {
                float proj = r2.p[k].x * axisX + r2.p[k].z * axisZ;
                if (proj < minB) minB = proj;
                if (proj > maxB) maxB = proj;
            }

            // Verifica se há separação (GAP)
            if (maxA < minB || maxB < minA) {
                return 0; // Separados! Não há colisão.
            }
        }
    }

    return 1; // Nenhuma separação encontrada, estão colidindo.
}

// Verifica colisão entre todas as partes do Player e todas as partes de UM inimigo
int checkPartsIntersection(
    RotatedRect pHull, RotatedRect pTurret, RotatedRect pPipe,
    RotatedRect eHull, RotatedRect eTurret, RotatedRect ePipe
) {
    // Testa Base do Player contra tudo do Inimigo
    if (checkSATCollision(pHull, eHull)) return 1;
    if (checkSATCollision(pHull, eTurret)) return 1;
    if (checkSATCollision(pHull, ePipe)) return 1;

    // Testa Torre do Player contra tudo do Inimigo
    if (checkSATCollision(pTurret, eHull)) return 1;
    if (checkSATCollision(pTurret, eTurret)) return 1;
    if (checkSATCollision(pTurret, ePipe)) return 1;

    // Testa Cano do Player contra tudo do Inimigo
    if (checkSATCollision(pPipe, eHull)) return 1;
    if (checkSATCollision(pPipe, eTurret)) return 1;
    if (checkSATCollision(pPipe, ePipe)) return 1;

    return 0;
}

// --- FUNÇÃO PRINCIPAL EXPOSTA ---

int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg) {
    
    // 1. Gerar os retângulos (OBBs) do Player na posição futura
    RotatedRect pHull = getRotatedRect(&hullModel.box, nextX, nextZ, hullAngleDeg);
    
    // Nota: Player usa turretAngle separado do hullAngle
    RotatedRect pTurret = getRotatedRect(&turretModel.box, nextX, nextZ, turretAngle);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, nextX, nextZ, turretAngle);

    // 2. Verificar contra todos os inimigos
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;

        float ex = enemies[i].x;
        float ez = enemies[i].z;
        
        // 3. Gerar os retângulos do Inimigo atual
        RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, enemies[i].hullAngle);
        
        // Nota: Inimigo soma os ângulos (conforme lógica de desenho do enemy.c)
        float enemyGlobalTurretAngle = enemies[i].hullAngle + enemies[i].turretAngle;
        
        RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, enemyGlobalTurretAngle);
        RotatedRect ePipe   = getRotatedRect(&enemyPipeModel.box, ex, ez, enemyGlobalTurretAngle);

        // 4. Teste SAT granular
        if (checkPartsIntersection(pHull, pTurret, pPipe, eHull, eTurret, ePipe)) {
            return 1; // Bateu!
        }
    }

    return 0;
}