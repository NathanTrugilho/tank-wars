#include "collision.h"
#include "tank.h"
#include "enemy.h"
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <GL/glut.h>

#define RADIAN_FACTOR (3.14159265359 / 180.0)

// --- MATEMÁTICA VETORIAL 3D ---

Point3D add(Point3D a, Point3D b) { return (Point3D){a.x + b.x, a.y + b.y, a.z + b.z}; }
Point3D sub(Point3D a, Point3D b) { return (Point3D){a.x - b.x, a.y - b.y, a.z - b.z}; }
float dot(Point3D a, Point3D b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Point3D normalizeVector(Point3D v) {
    float len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    if(len < 0.0001f) return (Point3D){0,0,0};
    return (Point3D){v.x/len, v.y/len, v.z/len};
}

Point3D rotateX(Point3D p, float angleDeg) {
    float rad = angleDeg * RADIAN_FACTOR; 
    float c = cos(rad);
    float s = sin(rad);
    return (Point3D){ p.x, p.y * c - p.z * s, p.y * s + p.z * c };
}

Point3D rotateY(Point3D p, float angleDeg) {
    float rad = -angleDeg * RADIAN_FACTOR; 
    float c = cos(rad);
    float s = sin(rad);
    return (Point3D){ p.x * c - p.z * s, p.y, p.x * s + p.z * c };
}

// --- GERAÇÃO DA CAIXA 3D (OBB) ---

CollisionBox getCollisionBox(const Box *localBox, float tx, float ty, float tz, 
                             float angleYaw, float anglePitch, 
                             float scaleW, float scaleL, 
                             float yMinFixed, float yMaxFixed) {
    CollisionBox cb;

    float width = (localBox->maxX - localBox->minX) * scaleW;
    float length = (localBox->maxZ - localBox->minZ) * scaleL;
    float height = yMaxFixed - yMinFixed;

    cb.halfSize[0] = width / 2.0f;
    cb.halfSize[1] = height / 2.0f;
    cb.halfSize[2] = length / 2.0f;

    float localCenterY = (yMinFixed + yMaxFixed) / 2.0f;
    
    Point3D localPos = {
        ((localBox->minX + localBox->maxX) / 2.0f) * scaleW,
        localCenterY - ty, 
        ((localBox->minZ + localBox->maxZ) / 2.0f) * scaleL
    };

    Point3D axisBase[3] = { {1,0,0}, {0,1,0}, {0,0,1} };
    
    for(int i=0; i<3; i++) {
        Point3D tmp = axisBase[i];
        if(fabs(anglePitch) > 0.001f) tmp = rotateX(tmp, anglePitch);
        tmp = rotateY(tmp, angleYaw);
        cb.axis[i] = normalizeVector(tmp);
    }

    Point3D centerRot = localPos;
    if(fabs(anglePitch) > 0.001f) centerRot = rotateX(centerRot, anglePitch);
    centerRot = rotateY(centerRot, angleYaw);

    cb.center.x = centerRot.x + tx;
    cb.center.y = centerRot.y + ty; 
    cb.center.z = centerRot.z + tz;

    int idx = 0;
    for(int i=-1; i<=1; i+=2) {
        for(int j=-1; j<=1; j+=2) {
            for(int k=-1; k<=1; k+=2) {
                Point3D v = cb.center;
                for(int d=0; d<3; d++) {
                    float s = (d==0 ? i : (d==1 ? j : k)) * cb.halfSize[d];
                    v.x += cb.axis[d].x * s;
                    v.y += cb.axis[d].y * s;
                    v.z += cb.axis[d].z * s;
                }
                cb.corners[idx++] = v;
            }
        }
    }
    return cb;
}

// --- COLISÃO SAT 3D ---

int getSeparation(CollisionBox *a, CollisionBox *b, Point3D axis) {
    float minA = FLT_MAX, maxA = -FLT_MAX;
    float minB = FLT_MAX, maxB = -FLT_MAX;

    for(int i=0; i<8; i++) {
        float p = dot(a->corners[i], axis);
        if(p < minA) minA = p;
        if(p > maxA) maxA = p;
    }
    for(int i=0; i<8; i++) {
        float p = dot(b->corners[i], axis);
        if(p < minB) minB = p;
        if(p > maxB) maxB = p;
    }
    return (maxA < minB || maxB < minA);
}

int checkCollisionOBB(CollisionBox *a, CollisionBox *b) {
    if(getSeparation(a, b, a->axis[0])) return 0;
    if(getSeparation(a, b, a->axis[1])) return 0;
    if(getSeparation(a, b, a->axis[2])) return 0;

    if(getSeparation(a, b, b->axis[0])) return 0;
    if(getSeparation(a, b, b->axis[1])) return 0;
    if(getSeparation(a, b, b->axis[2])) return 0;

    return 1;
}

// --- HELPERS ---

CollisionBox makeHull(float x, float z, float angle) {
    return getCollisionBox(&hullModel.box, x, tankY, z, angle, 0.0f, SCALE_HULL_W, SCALE_HULL_L, HULL_Y_MIN, HULL_Y_MAX);
}
CollisionBox makeTurret(float x, float z, float angle) {
    return getCollisionBox(&turretModel.box, x, tankY, z, angle, 0.0f, SCALE_TURRET_W, SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX);
}
CollisionBox makePipe(float x, float z, float tAngle, float pAngle) {
    return getCollisionBox(&pipeModel.box, x, tankY, z, tAngle, pAngle, SCALE_PIPE_W, SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX);
}

CollisionBox makeEnemyHull(Enemy *e) {
    return getCollisionBox(&enemyHullModel.box, e->x, tankY, e->z, e->hullAngle, 0.0f, ENEMY_SCALE_HULL_W, ENEMY_SCALE_HULL_L, HULL_Y_MIN, HULL_Y_MAX);
}
CollisionBox makeEnemyTurret(Enemy *e) {
    float angle = e->hullAngle + e->turretAngle;
    return getCollisionBox(&enemyTurretModel.box, e->x, tankY, e->z, angle, 0.0f, ENEMY_SCALE_TURRET_W, ENEMY_SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX);
}
CollisionBox makeEnemyPipe(Enemy *e) {
    float angle = e->hullAngle + e->turretAngle;
    return getCollisionBox(&enemyPipeModel.box, e->x, tankY, e->z, angle, 0.0f, ENEMY_SCALE_PIPE_W, ENEMY_SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX);
}

// --- FUNÇÕES PRINCIPAIS ---

int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg) {
    CollisionBox pHull = makeHull(nextX, nextZ, hullAngleDeg);
    CollisionBox pTurret = makeTurret(nextX, nextZ, turretAngle);
    CollisionBox pPipe = makePipe(nextX, nextZ, turretAngle, pipeAngle);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;
        
        CollisionBox eHull = makeEnemyHull(&enemies[i]);
        CollisionBox eTurret = makeEnemyTurret(&enemies[i]);
        CollisionBox ePipe = makeEnemyPipe(&enemies[i]);

        if (checkCollisionOBB(&pHull, &eHull) || checkCollisionOBB(&pHull, &eTurret) || checkCollisionOBB(&pHull, &ePipe)) return 1;
        if (checkCollisionOBB(&pTurret, &eHull) || checkCollisionOBB(&pTurret, &eTurret) || checkCollisionOBB(&pTurret, &ePipe)) return 1;
        if (checkCollisionOBB(&pPipe, &eHull) || checkCollisionOBB(&pPipe, &eTurret) || checkCollisionOBB(&pPipe, &ePipe)) return 1;
    }
    return 0;
}

int wouldCollideTurret(float nextTurretAngle) {
    CollisionBox pHull = makeHull(tankX, tankZ, hullAngle);
    CollisionBox pTurret = makeTurret(tankX, tankZ, nextTurretAngle);
    CollisionBox pPipe = makePipe(tankX, tankZ, nextTurretAngle, pipeAngle);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;
        
        CollisionBox eHull = makeEnemyHull(&enemies[i]);
        CollisionBox eTurret = makeEnemyTurret(&enemies[i]);
        CollisionBox ePipe = makeEnemyPipe(&enemies[i]);

        if (checkCollisionOBB(&pTurret, &eHull) || checkCollisionOBB(&pTurret, &eTurret) || checkCollisionOBB(&pTurret, &ePipe)) return 1;
        if (checkCollisionOBB(&pPipe, &eHull) || checkCollisionOBB(&pPipe, &eTurret) || checkCollisionOBB(&pPipe, &ePipe)) return 1;
        if (checkCollisionOBB(&pHull, &eHull) || checkCollisionOBB(&pHull, &eTurret) || checkCollisionOBB(&pHull, &ePipe)) return 1;
    }
    return 0;
}

// Esta função verifica se o inimigo 'enemyIndex', ao girar para 'nextTurretAngle',
// colide com o PLAYER ou com OUTROS INIMIGOS.
int wouldCollideEnemyTurret(int enemyIndex, float nextTurretAngle) {
    // 1. Cria o "Fantasma" do inimigo atual na nova posição/ângulo
    Enemy temp = enemies[enemyIndex];
    temp.turretAngle = nextTurretAngle;

    CollisionBox eHull = makeEnemyHull(&temp);
    CollisionBox eTurret = makeEnemyTurret(&temp);
    CollisionBox ePipe = makeEnemyPipe(&temp);

    // 2. Verifica Colisão contra o PLAYER
    CollisionBox pHull = makeHull(tankX, tankZ, hullAngle);
    CollisionBox pTurret = makeTurret(tankX, tankZ, turretAngle);
    CollisionBox pPipe = makePipe(tankX, tankZ, turretAngle, pipeAngle);

    if (checkCollisionOBB(&eHull, &pPipe) || checkCollisionOBB(&eTurret, &pPipe) || checkCollisionOBB(&ePipe, &pPipe)) return 1;
    if (checkCollisionOBB(&eTurret, &pHull) || checkCollisionOBB(&eTurret, &pTurret)) return 1;
    if (checkCollisionOBB(&ePipe, &pHull) || checkCollisionOBB(&ePipe, &pTurret)) return 1;

    // 3. --- NOVO: VERIFICAÇÃO INIMIGO vs INIMIGO ---
    for(int i = 0; i < MAX_ENEMIES; i++) {
        // Não checar contra si mesmo e nem contra inimigos mortos
        if (i == enemyIndex || !enemies[i].alive) continue;

        CollisionBox otherHull = makeEnemyHull(&enemies[i]);
        CollisionBox otherTurret = makeEnemyTurret(&enemies[i]);
        CollisionBox otherPipe = makeEnemyPipe(&enemies[i]);

        // Verifica se minhas partes (e...) batem nas partes do outro (other...)
        
        // Minha Torre bate no outro?
        if (checkCollisionOBB(&eTurret, &otherHull) || checkCollisionOBB(&eTurret, &otherTurret) || checkCollisionOBB(&eTurret, &otherPipe)) return 1;
        
        // Meu Cano bate no outro?
        if (checkCollisionOBB(&ePipe, &otherHull) || checkCollisionOBB(&ePipe, &otherTurret) || checkCollisionOBB(&ePipe, &otherPipe)) return 1;
        
        // (Opcional) Minha Base bate no outro? 
        // Se os inimigos estiverem parados e só girando a torre, a base não se mexe, 
        // mas se você adicionar movimento no futuro, é bom ter:
        if (checkCollisionOBB(&eHull, &otherHull) || checkCollisionOBB(&eHull, &otherTurret) || checkCollisionOBB(&eHull, &otherPipe)) return 1;
    }

    return 0;
}

// --- DEBUG VISUAL ---

void drawDebugBox(CollisionBox b) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // VERMELHO

    glBegin(GL_LINES);
    // Base
    glVertex3f(b.corners[0].x, b.corners[0].y, b.corners[0].z); glVertex3f(b.corners[1].x, b.corners[1].y, b.corners[1].z);
    glVertex3f(b.corners[1].x, b.corners[1].y, b.corners[1].z); glVertex3f(b.corners[3].x, b.corners[3].y, b.corners[3].z);
    glVertex3f(b.corners[3].x, b.corners[3].y, b.corners[3].z); glVertex3f(b.corners[2].x, b.corners[2].y, b.corners[2].z);
    glVertex3f(b.corners[2].x, b.corners[2].y, b.corners[2].z); glVertex3f(b.corners[0].x, b.corners[0].y, b.corners[0].z);
    
    // Topo
    glVertex3f(b.corners[4].x, b.corners[4].y, b.corners[4].z); glVertex3f(b.corners[5].x, b.corners[5].y, b.corners[5].z);
    glVertex3f(b.corners[5].x, b.corners[5].y, b.corners[5].z); glVertex3f(b.corners[7].x, b.corners[7].y, b.corners[7].z);
    glVertex3f(b.corners[7].x, b.corners[7].y, b.corners[7].z); glVertex3f(b.corners[6].x, b.corners[6].y, b.corners[6].z);
    glVertex3f(b.corners[6].x, b.corners[6].y, b.corners[6].z); glVertex3f(b.corners[4].x, b.corners[4].y, b.corners[4].z);
    
    // Laterais
    glVertex3f(b.corners[0].x, b.corners[0].y, b.corners[0].z); glVertex3f(b.corners[4].x, b.corners[4].y, b.corners[4].z);
    glVertex3f(b.corners[1].x, b.corners[1].y, b.corners[1].z); glVertex3f(b.corners[5].x, b.corners[5].y, b.corners[5].z);
    glVertex3f(b.corners[2].x, b.corners[2].y, b.corners[2].z); glVertex3f(b.corners[6].x, b.corners[6].y, b.corners[6].z);
    glVertex3f(b.corners[3].x, b.corners[3].y, b.corners[3].z); glVertex3f(b.corners[7].x, b.corners[7].y, b.corners[7].z);
    glEnd();

    glEnable(GL_LIGHTING); 
}

void debugDrawPlayerCollision() {
    drawDebugBox(makeHull(tankX, tankZ, hullAngle));
    drawDebugBox(makeTurret(tankX, tankZ, turretAngle));
    drawDebugBox(makePipe(tankX, tankZ, turretAngle, pipeAngle));
}

void debugDrawEnemyCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].alive) continue;
        drawDebugBox(makeEnemyHull(&enemies[i]));
        drawDebugBox(makeEnemyTurret(&enemies[i]));
        drawDebugBox(makeEnemyPipe(&enemies[i]));
    }
}