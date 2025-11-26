#include <tank.h>

Tank player;   // Agora tudo é centralizado aqui

const double RADIAN_FACTOR = 3.14159 / 180.0;

ObjModel turretModel, pipeModel, hullModel;

void drawTank() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(player.x, player.y, player.z);

        // HULL
        glPushMatrix();
            glRotatef(player.hullAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&hullModel);
            //drawBox(hullModel.box); // Hitbox da base
        glPopMatrix();

        // TURRET + PIPE
        glPushMatrix();
            glRotatef(player.turretAngle, 0.0f, 1.0f, 0.0f);
            drawModel(&turretModel);
            //drawBox(turretModel.box); //Hitbox da torreta

            glRotatef(player.pipeAngle, 1.0f, 0.0f, 0.0f);
            drawModel(&pipeModel);
            //drawBox(pipeModel.box); //Hitbox do cano
        glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void updateTank() {

    float nextX = player.x;
    float nextZ = player.z;
    float nextHullAngle = player.hullAngle;

    // ---------------------------
    //  MOVIMENTO DO HULL (W/S/A/D)
    // ---------------------------

    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
        nextZ -= cosf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
        nextZ += cosf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
    }
    if (keyStates['a'] || keyStates['A']) nextHullAngle += player.tankRotSpeed;
    if (keyStates['d'] || keyStates['D']) nextHullAngle -= player.tankRotSpeed;

    if (!wouldCollideTank(nextX, nextZ, nextHullAngle)) {
        player.x = nextX;
        player.z = nextZ;
        player.hullAngle = nextHullAngle;
    }

    // ---------------------------
    //  GIRO DA TORRETA
    // ---------------------------
    float nextTurretAngle = player.turretAngle;

    if (specialKeyStates[GLUT_KEY_LEFT])
        nextTurretAngle += player.tankRotSpeed; //turretRotSpeed?

    if (specialKeyStates[GLUT_KEY_RIGHT])
        nextTurretAngle -= player.tankRotSpeed; //turretRotSpeed?

    if (nextTurretAngle != player.turretAngle) {
        if (!wouldCollideTurret(nextTurretAngle)) {
            player.turretAngle = nextTurretAngle;
        }
    }

    // ---------------------------
    //  INCLINAÇÃO DO CANO
    // ---------------------------

    if (specialKeyStates[GLUT_KEY_UP]) {
        player.pipeAngle += player.pipeInclineSpeed;
        if (player.pipeAngle > MAX_PIPE_ANGLE)
            player.pipeAngle = MAX_PIPE_ANGLE;
    }

    if (specialKeyStates[GLUT_KEY_DOWN]) {
        player.pipeAngle -= player.pipeInclineSpeed;
        if (player.pipeAngle < MIN_PIPE_ANGLE)
            player.pipeAngle = MIN_PIPE_ANGLE;
    }

    updateMapCellPos();
    glutPostRedisplay();
}

void updateMapCellPos() {

    int posX_A = mapCells[player.mapCellZ][player.mapCellX].A.x;
    int posZ_A = mapCells[player.mapCellZ][player.mapCellX].A.z;
    int posX_D = mapCells[player.mapCellZ][player.mapCellX].D.x;
    int posZ_D = mapCells[player.mapCellZ][player.mapCellX].D.z;

    if (player.x < posX_A && player.mapCellX > 0)
        player.mapCellX--;
    else if (player.x > posX_D && player.mapCellX < 49)
        player.mapCellX++;

    if (player.z < posZ_A && player.mapCellZ > 0)
        player.mapCellZ--;
    else if (player.z > posZ_D && player.mapCellZ < 49)
        player.mapCellZ++;
}

void initTank() {

    if (!loadOBJ("objects/turret.obj", "objects/turret.mtl", &turretModel))
        printf("ERRO ao carregar modelo da torreta.\n");

    if (!loadOBJ("objects/pipe.obj", "objects/pipe.mtl", &pipeModel))
        printf("ERRO ao carregar modelo do canhão.\n");

    if (!loadOBJ("objects/hull.obj", "objects/hull.mtl", &hullModel))
        printf("ERRO ao carregar modelo da base.\n");

    // ---------------------------
    // Inicializando o Tank player
    // ---------------------------
    player.mapCellX = INITIAL_TANK_CELL_X;
    player.mapCellZ = INITIAL_TANK_CELL_Z;

    player.x = mapCells[player.mapCellZ][player.mapCellX].C.x;
    player.z = mapCells[player.mapCellZ][player.mapCellX].C.z;
    player.y = 0.5f;

    player.hullAngle = 0;
    player.turretAngle = 0;
    player.pipeAngle = 0;

    player.moveSpeed = TANK_MOVEMENT_SPEED;
    player.tankRotSpeed = TANK_ROT_SPEED;
    player.turretRotSpeed = TANK_ROT_SPEED;
    player.pipeInclineSpeed = PIPE_INCLINE_SPEED;

    player.health = 100;
    player.alive = 1;
    player.ammo = 100;
    player.bulletDmg = 33;
}
