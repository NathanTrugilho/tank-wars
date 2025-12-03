#include "tank.h"
#include "collision.h" // Inclui para usar CollisionBox, macros de escala e a função getCollisionBox
#include "map.h"       // Inclui para usar checkCollisionWithWorld

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
        glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void updateTank() {
    if (freeCameraMode) {
        glutPostRedisplay();
        return;
    }
    float nextX = player.x;
    float nextZ = player.z;
    float nextHullAngle = player.hullAngle;
    
    // inputs de movimento do tanque
    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
        nextZ -= cosf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
        nextZ += cosf(player.hullAngle * RADIAN_FACTOR) * player.moveSpeed;
    }
    if (keyStates['a'] || keyStates['A']) nextHullAngle += TANK_ROT_SPEED;
    if (keyStates['d'] || keyStates['D']) nextHullAngle -= TANK_ROT_SPEED;

    // Verificação de colisão antes de aplicar o movimento
    
    // Gera as caixas hipotéticas de TODAS as partes na nova posição (nextX, nextZ)
    CollisionBox nextHullBox = getCollisionBox(&hullModel.box, nextX, player.y, nextZ, 
                                               nextHullAngle, 0.0f, 
                                               SCALE_HULL_W, SCALE_HULL_L, HULL_Y_MIN, HULL_Y_MAX);

    // A torre e o cano giram com 'turretAngle', mas se movem para 'nextX/nextZ'
    CollisionBox nextTurretBox = getCollisionBox(&turretModel.box, nextX, player.y, nextZ, 
                                                 player.turretAngle, 0.0f, 
                                                 SCALE_TURRET_W, SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX);
                                                 
    CollisionBox nextPipeBox = getCollisionBox(&pipeModel.box, nextX, player.y, nextZ, 
                                               player.turretAngle, player.pipeAngle, 
                                               SCALE_PIPE_W, SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX);

    // Verifica colisão com o MUNDO ESTÁTICO (Prédios, etc)
    // Se a base, OU a torre, OU o cano baterem, o movimento é bloqueado.
    int hitWorld = checkCollisionWithWorld(&nextHullBox) || 
                   checkCollisionWithWorld(&nextTurretBox) || 
                   checkCollisionWithWorld(&nextPipeBox);

    // Verifica colisão com INIMIGOS e aplica movimento se livre
    if (!hitWorld && !wouldCollideTank(nextX, nextZ, nextHullAngle)) {
        player.x = nextX;
        player.z = nextZ;
        player.hullAngle = nextHullAngle;
    } 
    
    // Lógica da Torre
    float nextTurretAngle = player.turretAngle;

    if (specialKeyStates[GLUT_KEY_LEFT]) nextTurretAngle += TANK_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_RIGHT]) nextTurretAngle -= TANK_ROT_SPEED;

    // Se houve tentativa de girar a torre
    if (nextTurretAngle != player.turretAngle) {
        // Gera caixa hipotética da torre no novo angulo (mas posição atual player.x/player.z)
        CollisionBox rotTurretBox = getCollisionBox(&turretModel.box, player.x, player.y, player.z, 
                                                     nextTurretAngle, 0.0f, 
                                                     SCALE_TURRET_W, SCALE_TURRET_L, TURRET_Y_MIN, TURRET_Y_MAX);
        
        CollisionBox rotPipeBox = getCollisionBox(&pipeModel.box, player.x, player.y, player.z, 
                                                   nextTurretAngle, player.pipeAngle, 
                                                   SCALE_PIPE_W, SCALE_PIPE_L, PIPE_Y_MIN, PIPE_Y_MAX);

        // Verifica inimigos E mapa
        int colidiuInimigo = wouldCollideTurret(nextTurretAngle);
        int colidiuMapa = checkCollisionWithWorld(&rotTurretBox) || checkCollisionWithWorld(&rotPipeBox);

        if (!colidiuInimigo && !colidiuMapa) {
            player.turretAngle = nextTurretAngle;
        }
    }

    // Inclinação do Cano (Cima/Baixo)
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

    player.lastShootTime = 0;
    player.reloadTime = 3000;
    player.flagReloadCircle = 0;
}
