#include <tank.h>
#include <enemy.h>
#include <stdio.h>

Tank player;
extern Tank enemies[MAX_ENEMIES];

const double RADIAN_FACTOR = 3.14159 / 180.0; // Pra converter de graus pra radiano

void initTank(){
    player.alive = 1;
    // Caracteristicas do Tank
    player.tankAngle = 0.0f;
    player.moveSpeed = 0.2f;
    player.rotSpeed = 2.0f;
    // Posicao Incial
    player.mapCellX = INITIAL_TANK_CELL_X;
    player.mapCellZ = INITIAL_TANK_CELL_Z;
    player.position.x = mapCells[player.mapCellZ][player.mapCellX].C.x;
    player.position.y = 0.5f;
    player.position.z = mapCells[player.mapCellZ][player.mapCellX].C.z;

    // Hitbox
    // Inicialização SIMPLIFICADA da AABB (sem rotação, apenas posição)
    // O tamanho do tanque é 1.0 (X) x 0.5 (Y) x 2.0 (Z) na escala local.
    // Usamos o tamanho MÁXIMO (ex: 2.0x1.0x2.0) centrado no ponto de posição.
    player.hitbox.min.x = player.position.x - 0.5f;
    player.hitbox.max.x = player.position.x + 0.5f;
    
    player.hitbox.min.y = player.position.y - 0.25f; // Altura 0.5, centrado em Y=0.5
    player.hitbox.max.y = player.position.y + 0.25f;

    player.hitbox.min.z = player.position.z - 1.0f; // Comprimento 2.0
    player.hitbox.max.z = player.position.z + 1.0f;
}

void drawTank() {
    glPushMatrix();
        glTranslatef(player.position.x, player.position.y, player.position.z);
        glRotatef(player.tankAngle, 0.0f, 1.0f, 0.0f);

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
    float nextX = player.position.x;
    float nextZ = player.position.z;
    float moved = 0; // Flag para saber se tentamos um movimento linear

    // 1. Cálculo da PRÓXIMA POSIÇÃO (Proposta)
    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(player.tankAngle * RADIAN_FACTOR) * player.moveSpeed;
        nextZ -= cosf(player.tankAngle * RADIAN_FACTOR) * player.moveSpeed;
        moved = 1;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(player.tankAngle * RADIAN_FACTOR) * player.moveSpeed;
        nextZ += cosf(player.tankAngle * RADIAN_FACTOR) * player.moveSpeed;
        moved = 1;
    }
    
    // 2. Colisão Rotacional (Sempre permitida, a rotação não muda a AABB se não houver movimento)
    if (keyStates['a'] || keyStates['A']) {
        player.tankAngle += player.rotSpeed;
        moved = 1;
    }

    if (keyStates['d'] || keyStates['D']) {
        player.tankAngle -= player.rotSpeed;
        moved = 1;
    }

    // 3. Checagem de Colisão (apenas se houve tentativa de movimento)
    if (moved) {
        Vector3f nextPosition = {nextX, player.position.y, nextZ};
        AABB nextBox = getTankAABB(nextPosition);
        
        // Se NÃO houver colisão (checkPlayerCollision retorna 0), podemos mover.
        if (!checkPlayerCollision(nextBox)) {
            player.position.x = nextX;
            player.position.z = nextZ;
        } 
        // Se houver colisão, player.position.x e .z MANTÊM o valor antigo && não deixa rotacionar
    }

    updateMapCellPos();
}

void updateMapCellPos(){

    int posX_A = mapCells[player.mapCellZ][player.mapCellX].A.x;
    int posZ_A = mapCells[player.mapCellZ][player.mapCellX].A.z;
    int posX_D = mapCells[player.mapCellZ][player.mapCellX].D.x;
    int posZ_D = mapCells[player.mapCellZ][player.mapCellX].D.z;

    if(player.position.x < posX_A && player.mapCellX > 0) player.mapCellX--;
    else if(player.position.x > posX_D && player.mapCellX < 49) player.mapCellX++;

    if(player.position.z < posZ_A && player.mapCellZ > 0) player.mapCellZ--;
    else if(player.position.x > posZ_D && player.mapCellZ < 49) player.mapCellZ++;

    // Para fazer debug
    //printf("Celula %d %d \n", mapCellZ, mapCellX);

}

AABB getTankAABB(Vector3f position) {
    AABB box;
    // O tanque é 1.0 (X) x 0.5 (Y) x 2.0 (Z).
    box.min.x = position.x - 0.5f;
    box.max.x = position.x + 0.5f;
    
    box.min.y = position.y - 0.25f; 
    box.max.y = position.y + 0.25f;

    box.min.z = position.z - 1.0f;
    box.max.z = position.z + 1.0f;
    
    return box;
}

int checkPlayerCollision(AABB testBox) {
    // Itera sobre todos os inimigos
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
            // A AABB do inimigo é calculada em sua posição atual
            AABB enemyBox = getTankAABB(enemies[i].position); 

            if (checkCollisionAABB(testBox, enemyBox)) {
                // Colisão detectada!
                //printf("Tentativa de movimento bloqueada. Colisão com Inimigo %d.\n", i);
                return 1;
            }
        }
    }
    return 0; // Nenhuma colisão
}