#include <tank.h>

int mapCellX = INITIAL_TANK_CELL_X;
int mapCellZ = INITIAL_TANK_CELL_Z;

// Posição e orientação do tanque
float tankX;
float tankY;
float tankZ;

float tankAngle = 0.0f;

// Velocidade do tanque
float moveSpeed = 0.2f;
float rotSpeed = 2.0f;

const double RADIAN_FACTOR = 3.14159 / 180.0; // Pra converter de graus pra radiano

void drawTank() {
    glPushMatrix();
        glTranslatef(tankX, tankY, tankZ);
        glRotatef(tankAngle, 0.0f, 1.0f, 0.0f);

        glColor3f(0.7f, 0.1f, 0.1f);
        glPushMatrix();
            glScalef(1.0f, 0.5f, 2.0f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glColor3f(0.3f, 0.3f, 0.3f);
        glPushMatrix();
            glTranslatef(0.f, 0.3f, -0.8f); 
            glRotatef(-180, 1, 0, 0);
            GLUquadric* quad = gluNewQuadric();
            gluCylinder(quad, 0.1, 0.1, 1.0, 8, 2);
            gluDeleteQuadric(quad);
        glPopMatrix();
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
    tankX = mapCells[mapCellZ][mapCellX].C.x;
    tankZ = mapCells[mapCellZ][mapCellX].C.z;
    tankY = 0.5f;
}

