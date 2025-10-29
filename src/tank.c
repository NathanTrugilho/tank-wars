#include <tank.h>
#include <math.h>

// Declaração global do tanque principal
Tank playerTank;

const double RADIAN_FACTOR = 3.14159 / 180.0;

// Inicializa o tanque
void initTank(Tank *tank) {
    tank->mapCellX = INITIAL_TANK_CELL_X;
    tank->mapCellZ = INITIAL_TANK_CELL_Z;

    tank->hitbox.position.x = mapCells[tank->mapCellZ][tank->mapCellX].C.x;
    tank->hitbox.position.z = mapCells[tank->mapCellZ][tank->mapCellX].C.z;
    tank->hitbox.position.y = 0.5f;

    tank->tankAngle = 0.0f;
    tank->moveSpeed = 0.2f;
    tank->rotSpeed = 2.0f;
}

// Atualiza posição do tanque no mapa (troca de célula)
void updateMapCellPos(Tank *tank) {
    int posX_A = mapCells[tank->mapCellZ][tank->mapCellX].A.x;
    int posZ_A = mapCells[tank->mapCellZ][tank->mapCellX].A.z;
    int posX_D = mapCells[tank->mapCellZ][tank->mapCellX].D.x;
    int posZ_D = mapCells[tank->mapCellZ][tank->mapCellX].D.z;

    if (tank->hitbox.position.x < posX_A && tank->mapCellX > 0)
        tank->mapCellX--;
    else if (tank->hitbox.position.x > posX_D && tank->mapCellX < 49)
        tank->mapCellX++;

    if (tank->hitbox.position.z < posZ_A && tank->mapCellZ > 0)
        tank->mapCellZ--;
    else if (tank->hitbox.position.z > posZ_D && tank->mapCellZ < 49)
        tank->mapCellZ++;

    // printf("Célula atual: %d %d\n", tank->mapCellZ, tank->mapCellX);
}

// Atualiza movimento e rotação do tanque
void updateTank(Tank *tank) {
    float nextX = tank->hitbox.position.x;
    float nextZ = tank->hitbox.position.z;

    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(tank->tankAngle * RADIAN_FACTOR) * tank->moveSpeed;
        nextZ -= cosf(tank->tankAngle * RADIAN_FACTOR) * tank->moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(tank->tankAngle * RADIAN_FACTOR) * tank->moveSpeed;
        nextZ += cosf(tank->tankAngle * RADIAN_FACTOR) * tank->moveSpeed;
    }
    if (keyStates['a'] || keyStates['A'])
        tank->tankAngle += tank->rotSpeed;
    if (keyStates['d'] || keyStates['D'])
        tank->tankAngle -= tank->rotSpeed;

    tank->hitbox.position.x = nextX;
    tank->hitbox.position.z = nextZ;

    updateMapCellPos(tank);
}

// Desenha o tanque
void drawTank(const Tank *tank) {
    glPushMatrix();
        glTranslatef(tank->hitbox.position.x, tank->hitbox.position.y, tank->hitbox.position.z);
        glRotatef(tank->tankAngle, 0.0f, 1.0f, 0.0f);

        // Corpo principal
        glColor3f(0.7f, 0.1f, 0.1f);
        glPushMatrix();
            glScalef(1.0f, 0.5f, 2.0f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Cano
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
