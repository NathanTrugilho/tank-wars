#include "collision.h"
#include "tank.h"
#include "enemy.h"
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <GL/glut.h>

#define RADIAN_FACTOR (3.14159265359 / 180.0)

// Rotaciona um ponto em torno da Origem (0,0) do mundo local
Point2D rotatePoint(float x, float z, float angleRad) {
    // CORREÇÃO: Invertemos o ângulo para casar com o OpenGL
    // Se antes estava indo para a direita quando devia ir para a esquerda,
    // multiplicar por -1 resolve.
    angleRad = -angleRad; 

    Point2D res;
    float c = cos(angleRad);
    float s = sin(angleRad);
    
    res.x = x * c - z * s;
    res.z = x * s + z * c;
    return res;
}

// Gera o retângulo rotacionado respeitando o PIVÔ do modelo
RotatedRect getRotatedRect(const Box *localBox, float tx, float tz, float angleDeg, float scaleW, float scaleL) {
    RotatedRect rect;
    float angleRad = angleDeg * RADIAN_FACTOR;

    // 1. Pega os limites originais do OBJ
    float minX = localBox->minX;
    float maxX = localBox->maxX;
    float minZ = localBox->minZ;
    float maxZ = localBox->maxZ;

    // 2. Calcula o centro geométrico da caixa ATUAL
    float centerX = (minX + maxX) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    // 3. Calcula o tamanho (extents)
    float sizeX = maxX - minX;
    float sizeZ = maxZ - minZ;

    // 4. Aplica a escala (Shrink) em relação ao CENTRO DA CAIXA, 
    // mas mantém a posição da caixa relativa ao pivô (0,0).
    // Exemplo: Se o cano vai de Z=1 a Z=4 (centro 2.5), e encolhemos 50%,
    // ele passa a ir de Z=1.75 a Z=3.25. Ele NÃO volta para o zero.
    
    float halfNewW = (sizeX * scaleW) / 2.0f;
    float halfNewL = (sizeZ * scaleL) / 2.0f;

    // Coordenadas locais ajustadas
    float x1 = centerX - halfNewW;
    float x2 = centerX + halfNewW;
    float z1 = centerZ - halfNewL;
    float z2 = centerZ + halfNewL;

    // Vértices (sentido horário ou anti-horário não importa, desde que consistente)
    // Inferior-Esq, Superior-Esq, Superior-Dir, Inferior-Dir
    float localX[4] = { x1, x1, x2, x2 };
    float localZ[4] = { z1, z2, z2, z1 };

    for (int i = 0; i < 4; i++) {
        // 5. Rotaciona em torno da ORIGEM (0,0) do tanque
        Point2D p = rotatePoint(localX[i], localZ[i], angleRad);
        
        // 6. Translada para a posição no mundo
        rect.p[i].x = p.x + tx;
        rect.p[i].z = p.z + tz;
    }
    return rect;
}

// --- SAT (Separating Axis Theorem) - Mantido igual ---
int checkSATCollision(RotatedRect r1, RotatedRect r2) {
    RotatedRect *rects[2] = { &r1, &r2 };
    for (int i = 0; i < 2; i++) {
        RotatedRect *curr = rects[i];
        for (int j = 0; j < 2; j++) {
            Point2D p1 = curr->p[j];
            Point2D p2 = curr->p[(j + 1) % 4];
            float edgeX = p2.x - p1.x;
            float edgeZ = p2.z - p1.z;
            float axisX = -edgeZ;
            float axisZ = edgeX;
            // Normalização (importante para estabilidade numérica)
            float len = sqrt(axisX * axisX + axisZ * axisZ);
            if (len < 0.0001f) continue; // Evita div por zero
            axisX /= len; axisZ /= len;

            float minA = FLT_MAX, maxA = -FLT_MAX;
            for (int k = 0; k < 4; k++) {
                float proj = r1.p[k].x * axisX + r1.p[k].z * axisZ;
                if (proj < minA) minA = proj;
                if (proj > maxA) maxA = proj;
            }
            float minB = FLT_MAX, maxB = -FLT_MAX;
            for (int k = 0; k < 4; k++) {
                float proj = r2.p[k].x * axisX + r2.p[k].z * axisZ;
                if (proj < minB) minB = proj;
                if (proj > maxB) maxB = proj;
            }
            if (maxA < minB || maxB < minA) return 0; 
        }
    }
    return 1; 
}

// Função auxiliar de verificação em grupo
int checkPartsIntersection(
    RotatedRect *pHull, RotatedRect *pTurret, RotatedRect *pPipe,
    RotatedRect *eHull, RotatedRect *eTurret, RotatedRect *ePipe
) {
    if (checkSATCollision(*pHull, *eHull)) return 1;
    if (checkSATCollision(*pHull, *eTurret)) return 1;
    if (checkSATCollision(*pHull, *ePipe)) return 1;

    if (checkSATCollision(*pTurret, *eHull)) return 1;
    if (checkSATCollision(*pTurret, *eTurret)) return 1;
    if (checkSATCollision(*pTurret, *ePipe)) return 1;

    if (checkSATCollision(*pPipe, *eHull)) return 1;
    if (checkSATCollision(*pPipe, *eTurret)) return 1;
    if (checkSATCollision(*pPipe, *ePipe)) return 1;

    return 0;
}

// --- FUNÇÕES PRINCIPAIS (Atualizadas com escalas separadas) ---

int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg) {
    RotatedRect pHull   = getRotatedRect(&hullModel.box, nextX, nextZ, hullAngleDeg, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, nextX, nextZ, turretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, nextX, nextZ, turretAngle, SCALE_PIPE_W, SCALE_PIPE_L);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;
        float ex = enemies[i].x; float ez = enemies[i].z;
        
        RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, enemies[i].hullAngle, SCALE_HULL_W, SCALE_HULL_L);
        float eGlobT = enemies[i].hullAngle + enemies[i].turretAngle;
        RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, eGlobT, SCALE_TURRET_W, SCALE_TURRET_L);
        RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, eGlobT, SCALE_PIPE_W, SCALE_PIPE_L);

        if (checkPartsIntersection(&pHull, &pTurret, &pPipe, &eHull, &eTurret, &ePipe)) return 1;
    }
    return 0;
}

int wouldCollideTurret(float nextTurretAngle) {
    RotatedRect pHull   = getRotatedRect(&hullModel.box, tankX, tankZ, hullAngle, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, tankX, tankZ, nextTurretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, tankX, tankZ, nextTurretAngle, SCALE_PIPE_W, SCALE_PIPE_L);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;
        float ex = enemies[i].x; float ez = enemies[i].z;

        RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, enemies[i].hullAngle, SCALE_HULL_W, SCALE_HULL_L);
        float eGlobT = enemies[i].hullAngle + enemies[i].turretAngle;
        RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, eGlobT, SCALE_TURRET_W, SCALE_TURRET_L);
        RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, eGlobT, SCALE_PIPE_W, SCALE_PIPE_L);

        if (checkPartsIntersection(&pHull, &pTurret, &pPipe, &eHull, &eTurret, &ePipe)) return 1;
    }
    return 0;
}

int wouldCollideEnemyTurret(int enemyIndex, float nextTurretAngle) {
    float ex = enemies[enemyIndex].x;
    float ez = enemies[enemyIndex].z;
    float eh = enemies[enemyIndex].hullAngle;

    RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, eh, SCALE_HULL_W, SCALE_HULL_L);
    float propGlob = eh + nextTurretAngle;
    RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, propGlob, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, propGlob, SCALE_PIPE_W, SCALE_PIPE_L);

    // Player
    RotatedRect pHull = getRotatedRect(&hullModel.box, tankX, tankZ, hullAngle, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, tankX, tankZ, turretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe = getRotatedRect(&pipeModel.box, tankX, tankZ, turretAngle, SCALE_PIPE_W, SCALE_PIPE_L);

    if (checkPartsIntersection(&eHull, &eTurret, &ePipe, &pHull, &pTurret, &pPipe)) return 1;
    return 0;
}

// --- DEBUG VISUAL ---
// Adicione isso no seu game.c -> display() para ver as caixas!
void drawDebugHitbox(RotatedRect r) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho puro

    glBegin(GL_LINE_LOOP);
        for(int i=0; i<4; i++) {
            glVertex3f(r.p[i].x, 1.0f, r.p[i].z); // Desenhamos um pouco acima do chão (Y=1.0)
        }
    glEnd();
    
    glEnable(GL_LIGHTING); // Reativa luz se necessário
}

void debugDrawPlayerCollision() {
    RotatedRect pHull   = getRotatedRect(&hullModel.box, tankX, tankZ, hullAngle, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, tankX, tankZ, turretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, tankX, tankZ, turretAngle, SCALE_PIPE_W, SCALE_PIPE_L);
    
    drawDebugHitbox(pHull);
    drawDebugHitbox(pTurret);
    drawDebugHitbox(pPipe);
}