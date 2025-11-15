#include "collision.h"
#include "tank.h"
#include "enemy.h"
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <GL/glut.h>

#define RADIAN_FACTOR (3.14159265359 / 180.0)

// ... (manter rotatePoint igual) ...
Point2D rotatePoint(float x, float z, float angleRad) {
    angleRad = -angleRad; 
    Point2D res;
    float c = cos(angleRad);
    float s = sin(angleRad);
    res.x = x * c - z * s;
    res.z = x * s + z * c;
    return res;
}

// ... (manter getRotatedRect igual) ...
RotatedRect getRotatedRect(const Box *localBox, float tx, float tz, float angleDeg, float scaleW, float scaleL) {
    RotatedRect rect;
    float angleRad = angleDeg * RADIAN_FACTOR;
    float minX = localBox->minX; float maxX = localBox->maxX;
    float minZ = localBox->minZ; float maxZ = localBox->maxZ;
    float centerX = (minX + maxX) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;
    float sizeX = maxX - minX;
    float sizeZ = maxZ - minZ;
    float halfNewW = (sizeX * scaleW) / 2.0f;
    float halfNewL = (sizeZ * scaleL) / 2.0f;
    float x1 = centerX - halfNewW; float x2 = centerX + halfNewW;
    float z1 = centerZ - halfNewL; float z2 = centerZ + halfNewL;
    float localX[4] = { x1, x1, x2, x2 };
    float localZ[4] = { z1, z2, z2, z1 };
    for (int i = 0; i < 4; i++) {
        Point2D p = rotatePoint(localX[i], localZ[i], angleRad);
        rect.p[i].x = p.x + tx;
        rect.p[i].z = p.z + tz;
    }
    return rect;
}

// ... (manter checkSATCollision igual) ...
int checkSATCollision(RotatedRect r1, RotatedRect r2) {
    RotatedRect *rects[2] = { &r1, &r2 };
    for (int i = 0; i < 2; i++) {
        RotatedRect *curr = rects[i];
        for (int j = 0; j < 2; j++) {
            Point2D p1 = curr->p[j];
            Point2D p2 = curr->p[(j + 1) % 4];
            float edgeX = p2.x - p1.x; float edgeZ = p2.z - p1.z;
            float axisX = -edgeZ; float axisZ = edgeX;
            float len = sqrt(axisX * axisX + axisZ * axisZ);
            if (len < 0.0001f) continue;
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

// --- NOVO: VERIFICAÇÃO DE ALTURA ---
// Retorna 1 se houver sobreposição vertical, 0 caso contrário.
int checkVerticalOverlap(float minY1, float maxY1, float minY2, float maxY2) {
    // Lógica clássica de intersecção 1D
    return (minY1 <= maxY2 && maxY1 >= minY2);
}

// Função auxiliar que combina Altura + SAT
int checkCollision3D(RotatedRect *r1, float min1, float max1, 
                     RotatedRect *r2, float min2, float max2) {
    // 1. Primeiro teste rápido: Altura bate?
    if (!checkVerticalOverlap(min1, max1, min2, max2)) {
        return 0; // Alturas diferentes, nem precisa testar X/Z!
    }
    // 2. Teste caro: Geometria bate?
    return checkSATCollision(*r1, *r2);
}

// Verifica intersecção entre todas as partes considerando ALTURA
int checkPartsIntersection(
    RotatedRect *pHull, RotatedRect *pTurret, RotatedRect *pPipe,
    RotatedRect *eHull, RotatedRect *eTurret, RotatedRect *ePipe
) {
    // --- PLAYER HULL vs INIMIGO ... ---
    if (checkCollision3D(pHull, HULL_Y_MIN, HULL_Y_MAX, eHull, HULL_Y_MIN, HULL_Y_MAX)) return 1;
    if (checkCollision3D(pHull, HULL_Y_MIN, HULL_Y_MAX, eTurret, TURRET_Y_MIN, TURRET_Y_MAX)) return 1;
    if (checkCollision3D(pHull, HULL_Y_MIN, HULL_Y_MAX, ePipe, PIPE_Y_MIN, PIPE_Y_MAX)) return 1;

    // --- PLAYER TURRET vs INIMIGO ... ---
    if (checkCollision3D(pTurret, TURRET_Y_MIN, TURRET_Y_MAX, eHull, HULL_Y_MIN, HULL_Y_MAX)) return 1;
    if (checkCollision3D(pTurret, TURRET_Y_MIN, TURRET_Y_MAX, eTurret, TURRET_Y_MIN, TURRET_Y_MAX)) return 1;
    if (checkCollision3D(pTurret, TURRET_Y_MIN, TURRET_Y_MAX, ePipe, PIPE_Y_MIN, PIPE_Y_MAX)) return 1;

    // --- PLAYER PIPE vs INIMIGO ... ---
    // AQUI ESTÁ A MÁGICA: 
    // checkCollision3D(pPipe, ..., eHull, ...) vai comparar PIPE_MIN(1.0) com HULL_MAX(0.9).
    // Como 1.0 > 0.9, não há overlap vertical. A colisão é ignorada!
    
    if (checkCollision3D(pPipe, PIPE_Y_MIN, PIPE_Y_MAX, eHull, HULL_Y_MIN, HULL_Y_MAX)) return 1;
    if (checkCollision3D(pPipe, PIPE_Y_MIN, PIPE_Y_MAX, eTurret, TURRET_Y_MIN, TURRET_Y_MAX)) return 1;
    if (checkCollision3D(pPipe, PIPE_Y_MIN, PIPE_Y_MAX, ePipe, PIPE_Y_MIN, PIPE_Y_MAX)) return 1;

    return 0;
}

// --- AS FUNÇÕES PRINCIPAIS FICAM IGUAIS (Elas só chamam checkPartsIntersection) ---

int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg) {
    // PLAYER usa constantes de collision.h
    RotatedRect pHull   = getRotatedRect(&hullModel.box, nextX, nextZ, hullAngleDeg, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, nextX, nextZ, turretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, nextX, nextZ, turretAngle, SCALE_PIPE_W, SCALE_PIPE_L);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;
        float ex = enemies[i].x; float ez = enemies[i].z;
        
        // INIMIGO usa constantes de enemy.h
        RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, enemies[i].hullAngle, ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L);
        
        float eGlobT = enemies[i].hullAngle + enemies[i].turretAngle;
        RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, eGlobT, ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L);
        RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, eGlobT, ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L);

        // Passamos as alturas corretas aqui também
        if (checkCollision3D(&pHull, HULL_Y_MIN, HULL_Y_MAX, &eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX)) return 1;
        if (checkCollision3D(&pHull, HULL_Y_MIN, HULL_Y_MAX, &eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX)) return 1;
        if (checkCollision3D(&pHull, HULL_Y_MIN, HULL_Y_MAX, &ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX)) return 1;
        
        // Repetir logica para pTurret e pPipe contra as partes do inimigo...
        // Simplifiquei aqui chamando checkPartsIntersection se você adaptou ela para aceitar alturas,
        // mas como as alturas agora são diferentes (Player vs Enemy), o ideal é fazer checkCollision3D direto.
        
        // Teste Player Turret
        if (checkCollision3D(&pTurret, TURRET_Y_MIN, TURRET_Y_MAX, &eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX)) return 1;
        if (checkCollision3D(&pTurret, TURRET_Y_MIN, TURRET_Y_MAX, &eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX)) return 1;
        if (checkCollision3D(&pTurret, TURRET_Y_MIN, TURRET_Y_MAX, &ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX)) return 1;

        // Teste Player Pipe
        if (checkCollision3D(&pPipe, PIPE_Y_MIN, PIPE_Y_MAX, &eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX)) return 1;
        if (checkCollision3D(&pPipe, PIPE_Y_MIN, PIPE_Y_MAX, &eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX)) return 1;
        if (checkCollision3D(&pPipe, PIPE_Y_MIN, PIPE_Y_MAX, &ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX)) return 1;
    }
    return 0;
}
int wouldCollideTurret(float nextTurretAngle) {
    // 1. Gera retângulos do PLAYER
    // A Base (Hull) usa a posição e ângulo ATUAIS (não estamos movendo a base)
    RotatedRect pHull   = getRotatedRect(&hullModel.box, tankX, tankZ, hullAngle, SCALE_HULL_W, SCALE_HULL_L);
    
    // A Torre e o Cano usam o PRÓXIMO ângulo (nextTurretAngle) para prever a colisão
    RotatedRect pTurret = getRotatedRect(&turretModel.box, tankX, tankZ, nextTurretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, tankX, tankZ, nextTurretAngle, SCALE_PIPE_W, SCALE_PIPE_L);

    // 2. Verifica contra todos os inimigos
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;

        float ex = enemies[i].x; 
        float ez = enemies[i].z;

        // Gera retângulos do INIMIGO usando as constantes DELE (ENEMY_SCALE_...)
        RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, enemies[i].hullAngle, ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L);
        
        float eGlobT = enemies[i].hullAngle + enemies[i].turretAngle;
        RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, eGlobT, ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L);
        RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, eGlobT, ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L);

        // 3. Verifica colisões cruzadas (Player vs Enemy) considerando ALTURAS (Y-Range)

        // --- Player HULL vs Inimigo ---
        if (checkCollision3D(&pHull, HULL_Y_MIN, HULL_Y_MAX, &eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX)) return 1;
        if (checkCollision3D(&pHull, HULL_Y_MIN, HULL_Y_MAX, &eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX)) return 1;
        if (checkCollision3D(&pHull, HULL_Y_MIN, HULL_Y_MAX, &ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX)) return 1;

        // --- Player TURRET vs Inimigo ---
        if (checkCollision3D(&pTurret, TURRET_Y_MIN, TURRET_Y_MAX, &eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX)) return 1;
        if (checkCollision3D(&pTurret, TURRET_Y_MIN, TURRET_Y_MAX, &eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX)) return 1;
        if (checkCollision3D(&pTurret, TURRET_Y_MIN, TURRET_Y_MAX, &ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX)) return 1;

        // --- Player PIPE vs Inimigo ---
        if (checkCollision3D(&pPipe, PIPE_Y_MIN, PIPE_Y_MAX, &eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX)) return 1;
        if (checkCollision3D(&pPipe, PIPE_Y_MIN, PIPE_Y_MAX, &eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX)) return 1;
        if (checkCollision3D(&pPipe, PIPE_Y_MIN, PIPE_Y_MAX, &ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX)) return 1;
    }

    return 0; // Nenhuma colisão detectada
}
int wouldCollideEnemyTurret(int enemyIndex, float nextTurretAngle) {
    float ex = enemies[enemyIndex].x;
    float ez = enemies[enemyIndex].z;
    float eh = enemies[enemyIndex].hullAngle;

    // Gera retângulos do INIMIGO alvo (quem está se mexendo)
    RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, eh, ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L);
    float propGlob = eh + nextTurretAngle;
    RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, propGlob, ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L);
    RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, propGlob, ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L);

    // Gera retângulos do PLAYER (obstáculo)
    RotatedRect pHull = getRotatedRect(&hullModel.box, tankX, tankZ, hullAngle, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, tankX, tankZ, turretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe = getRotatedRect(&pipeModel.box, tankX, tankZ, turretAngle, SCALE_PIPE_W, SCALE_PIPE_L);

    // Verifica colisão usando as alturas CORRETAS de cada um
    // Inimigo (e...) vs Player (p...)
    
    // eHull vs Player
    if (checkCollision3D(&eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX, &pHull, HULL_Y_MIN, HULL_Y_MAX)) return 1;
    if (checkCollision3D(&eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX, &pTurret, TURRET_Y_MIN, TURRET_Y_MAX)) return 1;
    if (checkCollision3D(&eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX, &pPipe, PIPE_Y_MIN, PIPE_Y_MAX)) return 1;

    // eTurret vs Player
    if (checkCollision3D(&eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX, &pHull, HULL_Y_MIN, HULL_Y_MAX)) return 1;
    if (checkCollision3D(&eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX, &pTurret, TURRET_Y_MIN, TURRET_Y_MAX)) return 1;
    if (checkCollision3D(&eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX, &pPipe, PIPE_Y_MIN, PIPE_Y_MAX)) return 1;

    // ePipe vs Player
    if (checkCollision3D(&ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX, &pHull, HULL_Y_MIN, HULL_Y_MAX)) return 1;
    if (checkCollision3D(&ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX, &pTurret, TURRET_Y_MIN, TURRET_Y_MAX)) return 1;
    if (checkCollision3D(&ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX, &pPipe, PIPE_Y_MIN, PIPE_Y_MAX)) return 1;

    return 0;
}
// --- DEBUG VISUAL ATUALIZADO (Desenha na altura certa) ---
void drawDebugHitbox(RotatedRect r, float yMin, float yMax) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho

    // Desenha a base (yMin)
    glBegin(GL_LINE_LOOP);
        for(int i=0; i<4; i++) glVertex3f(r.p[i].x, yMin, r.p[i].z);
    glEnd();

    // Desenha o topo (yMax)
    glBegin(GL_LINE_LOOP);
        for(int i=0; i<4; i++) glVertex3f(r.p[i].x, yMax, r.p[i].z);
    glEnd();

    // Conecta os cantos (linhas verticais)
    glBegin(GL_LINES);
        for(int i=0; i<4; i++) {
            glVertex3f(r.p[i].x, yMin, r.p[i].z);
            glVertex3f(r.p[i].x, yMax, r.p[i].z);
        }
    glEnd();
    
    glEnable(GL_LIGHTING); 
}

void debugDrawPlayerCollision() {
    RotatedRect pHull   = getRotatedRect(&hullModel.box, tankX, tankZ, hullAngle, SCALE_HULL_W, SCALE_HULL_L);
    RotatedRect pTurret = getRotatedRect(&turretModel.box, tankX, tankZ, turretAngle, SCALE_TURRET_W, SCALE_TURRET_L);
    RotatedRect pPipe   = getRotatedRect(&pipeModel.box, tankX, tankZ, turretAngle, SCALE_PIPE_W, SCALE_PIPE_L);
    
    drawDebugHitbox(pHull, HULL_Y_MIN, HULL_Y_MAX);
    drawDebugHitbox(pTurret, TURRET_Y_MIN, TURRET_Y_MAX);
    drawDebugHitbox(pPipe, PIPE_Y_MIN, PIPE_Y_MAX);
}
void debugDrawEnemyCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;

        float ex = enemies[i].x;
        float ez = enemies[i].z;
        
        // Recalcula as caixas usando as constantes do ENEMY.H
        RotatedRect eHull = getRotatedRect(&enemyHullModel.box, ex, ez, enemies[i].hullAngle, ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L);
        
        float eGlobT = enemies[i].hullAngle + enemies[i].turretAngle;
        RotatedRect eTurret = getRotatedRect(&enemyTurretModel.box, ex, ez, eGlobT, ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L);
        RotatedRect ePipe = getRotatedRect(&enemyPipeModel.box, ex, ez, eGlobT, ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L);

        // Desenha usando as alturas do ENEMY.H
        drawDebugHitbox(eHull, ENEMY_HULL_Y_MIN, ENEMY_HULL_Y_MAX);
        drawDebugHitbox(eTurret, ENEMY_TURRET_Y_MIN, ENEMY_TURRET_Y_MAX);
        drawDebugHitbox(ePipe, ENEMY_PIPE_Y_MIN, ENEMY_PIPE_Y_MAX);
    }
}