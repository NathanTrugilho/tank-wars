#include <tank.h>

int mapCellX = INITIAL_TANK_CELL_X;
int mapCellZ = INITIAL_TANK_CELL_Z;

ObjModel turretModel, pipeModel, hullModel;

float tankX;
float tankY;
float tankZ;

float hullAngle = 0.0f;
float turretAngle = 0.0f;
float pipeAngle = 0.0f;

float moveSpeed = 0.1f;

float rotSpeed = 2.0f;
float turretRotSpeed = 1.0f;

float pipeInclSpeed = 0.5f;

const double RADIAN_FACTOR = 3.14159 / 180.0; // Pra converter de graus pra radiano

void drawTank() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(tankX, tankY, tankZ);
        glPushMatrix();
            glRotatef(hullAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&hullModel); 
            drawBox(hullModel.box); // Hitbox da base
        glPopMatrix();

        glPushMatrix();
            glRotatef(turretAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&turretModel);
            drawBox(turretModel.box); //Hitbox da torreta
            glRotatef(pipeAngle, 1.0f, 0.0f, 0.0f); // Em x
            drawModel(&pipeModel);
            drawBox(pipeModel.box); //Hitbox do cano
        glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D); // Tem que desativar se não fica tudo escuro pq as outras coisas n têm textura
}

void updateTank() {
    float nextX = tankX;
    float nextZ = tankZ;

    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(hullAngle * RADIAN_FACTOR) * moveSpeed;
        nextZ -= cosf(hullAngle * RADIAN_FACTOR) * moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(hullAngle * RADIAN_FACTOR) * moveSpeed;
        nextZ += cosf(hullAngle * RADIAN_FACTOR) * moveSpeed;
    }
    if (keyStates['a'] || keyStates['A']) hullAngle += rotSpeed;
    if (keyStates['d'] || keyStates['D']) hullAngle -= rotSpeed;

    if (specialKeyStates[GLUT_KEY_LEFT]) turretAngle += rotSpeed;
    if (specialKeyStates[GLUT_KEY_RIGHT]) turretAngle -= rotSpeed;

    if (specialKeyStates[GLUT_KEY_UP]) {
        pipeAngle += pipeInclSpeed;
        if (pipeAngle > MAX_PIPE_ANGLE) pipeAngle = MAX_PIPE_ANGLE; // limite superior
    }
    if (specialKeyStates[GLUT_KEY_DOWN]) {
        pipeAngle -= pipeInclSpeed;
        if (pipeAngle < MIN_PIPE_ANGLE) pipeAngle = MIN_PIPE_ANGLE; // limite inferior
    }

    if (!wouldCollideTank(nextX, nextZ, hullAngle)) {
        tankX = nextX;
        tankZ = nextZ;
    } else {
        // Para debug: imprime ou trata o bloqueio do movimento
        printf("Movimento bloqueado por colisão\n");
    }

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

