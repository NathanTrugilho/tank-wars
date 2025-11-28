#include "tank.h"
#include "collision.h" // Inclui para usar CollisionBox, macros de escala e a função getCollisionBox
#include "map.h"       // Inclui para usar checkCollisionWithWorld

int mapCellX = INITIAL_TANK_CELL_X;
int mapCellZ = INITIAL_TANK_CELL_Z;

const double RADIAN_FACTOR = 3.14159 / 180.0;

ObjModel turretModel, pipeModel, hullModel;

float tankX;
float tankY;
float tankZ;

float hullAngle = 0.0f;
float turretAngle = 0.0f;
float pipeAngle = 0.0f;

void drawTank() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(tankX, tankY, tankZ);
        glPushMatrix();
            glRotatef(hullAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&hullModel); 
        glPopMatrix();

        glPushMatrix();
            glRotatef(turretAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&turretModel);
            glRotatef(pipeAngle, 1.0f, 0.0f, 0.0f); // Em x
            drawModel(&pipeModel);
        glPopMatrix();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D); 
}
void updateTank() {
    if (freeCameraMode) {
        glutPostRedisplay();
        return;
    }
    float nextX = tankX;
    float nextZ = tankZ;
    float nextHullAngle = hullAngle;
    
    // inputs de movimento do tanque
    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(hullAngle * RADIAN_FACTOR) * TANK_MOVEMENT_SPEED;
        nextZ -= cosf(hullAngle * RADIAN_FACTOR) * TANK_MOVEMENT_SPEED;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(hullAngle * RADIAN_FACTOR) * TANK_MOVEMENT_SPEED;
        nextZ += cosf(hullAngle * RADIAN_FACTOR) * TANK_MOVEMENT_SPEED;
    }
    if (keyStates['a'] || keyStates['A']) nextHullAngle += TANK_ROT_SPEED;
    if (keyStates['d'] || keyStates['D']) nextHullAngle -= TANK_ROT_SPEED;

    // Verificação de colisão antes de aplicar o movimento
    
    // Gera as caixas hipotéticas de TODAS as partes na nova posição (nextX, nextZ)
    CollisionBox nextHullBox = getCollisionBox(&hullModel.box, nextX, tankY, nextZ, 
                                               nextHullAngle, 0.0f, 
                                               SCALE_HULL_W, SCALE_HULL_L, HULL_Y_MIN, HULL_Y_MAX);

    // A torre e o cano giram com 'turretAngle', mas se movem para 'nextX/nextZ'
    CollisionBox nextTurretBox = getCollisionBox(&turretModel.box, nextX, tankY, nextZ, 
                                                 turretAngle, 0.0f, 
                                                 SCALE_TURRET_W, SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX);
                                                 
    CollisionBox nextPipeBox = getCollisionBox(&pipeModel.box, nextX, tankY, nextZ, 
                                               turretAngle, pipeAngle, 
                                               SCALE_PIPE_W, SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX);

    // Verifica colisão com o MUNDO ESTÁTICO (Prédios, etc)
    // Se a base, OU a torre, OU o cano baterem, o movimento é bloqueado.
    int hitWorld = checkCollisionWithWorld(&nextHullBox) || 
                   checkCollisionWithWorld(&nextTurretBox) || 
                   checkCollisionWithWorld(&nextPipeBox);

    // Verifica colisão com INIMIGOS e aplica movimento se livre
    if (!hitWorld && !wouldCollideTank(nextX, nextZ, nextHullAngle)) {
        tankX = nextX;
        tankZ = nextZ;
        hullAngle = nextHullAngle;
    } 
    
    // Lógica da Torre
    float nextTurretAngle = turretAngle;

    if (specialKeyStates[GLUT_KEY_LEFT]) nextTurretAngle += TANK_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_RIGHT]) nextTurretAngle -= TANK_ROT_SPEED;

    // Se houve tentativa de girar a torre
    if (nextTurretAngle != turretAngle) {
        // Gera caixa hipotética da torre no novo angulo (mas posição atual tankX/tankZ)
        CollisionBox rotTurretBox = getCollisionBox(&turretModel.box, tankX, tankY, tankZ, 
                                                     nextTurretAngle, 0.0f, 
                                                     SCALE_TURRET_W, SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX);
        
        CollisionBox rotPipeBox = getCollisionBox(&pipeModel.box, tankX, tankY, tankZ, 
                                                   nextTurretAngle, pipeAngle, 
                                                   SCALE_PIPE_W, SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX);

        // Verifica inimigos E mapa
        int colidiuInimigo = wouldCollideTurret(nextTurretAngle);
        int colidiuMapa = checkCollisionWithWorld(&rotTurretBox) || checkCollisionWithWorld(&rotPipeBox);

        if (!colidiuInimigo && !colidiuMapa) {
            turretAngle = nextTurretAngle;
        }
    }

    // Inclinação do Cano (Cima/Baixo)
    if (specialKeyStates[GLUT_KEY_UP]) {
        pipeAngle += PIPE_INCLINE_SPEED;
        if (pipeAngle > MAX_PIPE_ANGLE) pipeAngle = MAX_PIPE_ANGLE; 
    }
    if (specialKeyStates[GLUT_KEY_DOWN]) {
        pipeAngle -= PIPE_INCLINE_SPEED;
        if (pipeAngle < MIN_PIPE_ANGLE) pipeAngle = MIN_PIPE_ANGLE; 
    }

    updateMapCellPos();
    glutPostRedisplay();
}

void updateMapCellPos(){
    int posX_A = mapCells[mapCellZ][mapCellX].A.x;
    int posZ_A = mapCells[mapCellZ][mapCellX].A.z;
    int posX_D = mapCells[mapCellZ][mapCellX].D.x;
    int posZ_D = mapCells[mapCellZ][mapCellX].D.z;

    if(tankX < posX_A && mapCellX > 0) mapCellX--;
    else if(tankX > posX_D && mapCellX < 49) mapCellX++;

    if(tankZ < posZ_A && mapCellZ > 0) mapCellZ--;
    else if(tankX > posZ_D && mapCellZ < 49) mapCellZ++;
}

void initTank(){
    if (loadOBJ("objects/turret.obj", "objects/turret.mtl", &turretModel)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da torreta.\n");
    }
    if (loadOBJ("objects/pipe.obj", "objects/pipe.mtl", &pipeModel)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo do canhão.\n");
    }
    if (loadOBJ("objects/hull.obj", "objects/hull.mtl", &hullModel)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da base.\n");
    }

    tankX = mapCells[mapCellZ][mapCellX].C.x;
    tankZ = mapCells[mapCellZ][mapCellX].C.z;
    tankY = 0.5f;
}