#include <tank.h>

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
            //drawBox(hullModel.box); // Hitbox da base
        glPopMatrix();

        glPushMatrix();
            glRotatef(turretAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&turretModel);
            //drawBox(turretModel.box); //Hitbox da torreta
            glRotatef(pipeAngle, 1.0f, 0.0f, 0.0f); // Em x
            drawModel(&pipeModel);
            //drawBox(pipeModel.box); //Hitbox do cano
        glPopMatrix();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D); // Tem que desativar se não fica tudo escuro pq as outras coisas n têm textura
}

void updateTank() {
    float nextX = tankX;
    float nextZ = tankZ;
    float nextHullAngle = hullAngle;
    
    // --- 1. Lógica de Movimento do Corpo (W/S/A/D) ---
    
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

    // Verificação Unificada:
    // Se eu me mover para nextX/nextZ ou girar o corpo para nextHullAngle,
    // alguma parte do meu tanque (base OU cano) vai bater?
    // Nota: A função wouldCollideTank usa a 'turretAngle' global atual.
    // Como a torre anda junto com o tanque, se o tanque anda e o cano bate, wouldCollideTank retorna 1.
    if (!wouldCollideTank(nextX, nextZ, nextHullAngle)) {
        tankX = nextX;
        tankZ = nextZ;
        hullAngle = nextHullAngle;
    } 

    
    float nextTurretAngle = turretAngle;

    if (specialKeyStates[GLUT_KEY_LEFT]) nextTurretAngle += TANK_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_RIGHT]) nextTurretAngle -= TANK_ROT_SPEED;

    // Se houve tentativa de girar a torre
    if (nextTurretAngle != turretAngle) {
        // Verifica se SOMENTE girar a torre causaria colisão
        if (!wouldCollideTurret(nextTurretAngle)) {
            turretAngle = nextTurretAngle;
        }
    }

    // --- 3. Inclinação do Cano (Cima/Baixo) ---
    // Isso não afeta colisão X/Z, pode manter direto
    if (specialKeyStates[GLUT_KEY_UP]) {
        pipeAngle += PIPE_INCLINE_SPEED;
        if (pipeAngle > MAX_PIPE_ANGLE) pipeAngle = MAX_PIPE_ANGLE; 
    }
    if (specialKeyStates[GLUT_KEY_DOWN]) {
        pipeAngle -= PIPE_INCLINE_SPEED;
        if (pipeAngle < MIN_PIPE_ANGLE) pipeAngle = MIN_PIPE_ANGLE; 
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

