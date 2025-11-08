#include <tank.h>

int mapCellX = INITIAL_TANK_CELL_X;
int mapCellZ = INITIAL_TANK_CELL_Z;

// Posição e orientação do tanque
float tankX;
float tankY;
float tankZ;

float tankAngle = 0.0f;

ObjModel turretModel, cannonModel, hullModel;

// Velocidade do tanque
float moveSpeed = 0.2f;
float rotSpeed = 2.0f;

const double RADIAN_FACTOR = 3.14159 / 180.0; // Pra converter de graus pra radiano

void drawTank() {
    glPushMatrix();
        glTranslatef(tankX, tankY, tankZ);
        glRotatef(tankAngle, 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f,0.0f,0.0f);
        drawModel(&hullModel);
        drawModel(&cannonModel);
        drawModel(&turretModel);
    glPopMatrix();
}

// Atualiza tanque
void updateTank() {
    float nextX = tankX;
    float nextZ = tankZ;

    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(tankAngle * RADIAN_FACTOR) * moveSpeed;
        nextZ -= cosf(tankAngle * RADIAN_FACTOR) * moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(tankAngle * RADIAN_FACTOR) * moveSpeed;
        nextZ += cosf(tankAngle * RADIAN_FACTOR) * moveSpeed;
    }
    if (keyStates['a'] || keyStates['A']) tankAngle += rotSpeed;
    if (keyStates['d'] || keyStates['D']) tankAngle -= rotSpeed;

    tankX = nextX;
    tankZ = nextZ;

    updateMapCellPos();
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

    // Para fazer debug
    //printf("Celula %d %d \n", mapCellZ, mapCellX);

}

void initTank(){

    if (loadOBJ("objects/turret.obj", "objects/turret.mtl", &turretModel)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da torreta.\n");
    }

    if (loadOBJ("objects/cannon.obj", "objects/cannon.mtl", &cannonModel)) {
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

