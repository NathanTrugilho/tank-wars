#include <map.h>

square mapCells[MAP_SIZE][MAP_SIZE];

int heightMatrix[MAP_SIZE][MAP_SIZE];

void initHeightMatrix(){

    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            heightMatrix[z][x] = 0.0f;
        }
    }

    for (int x = 0; x < MAP_SIZE; x++) {
        heightMatrix[0][x] = 1.0f;
        heightMatrix[1][x] = 1.0f;
        heightMatrix[2][x] = 1.0f;
        heightMatrix[3][x] = 1.0f;
        heightMatrix[4][x] = 0.8f;
        heightMatrix[5][x] = 0.7f;
        heightMatrix[6][x] = 0.6f;
        heightMatrix[7][x] = 0.5f;
        heightMatrix[8][x] = 0.4f;
        heightMatrix[9][x] = 0.3f;
        heightMatrix[10][x] = 0.2f;
        heightMatrix[11][x] = 0.1f;
    }
}

void initMapCells(){

    initHeightMatrix();

    float coordZ = -MAP_SIZE / 2;
    float coordX = -MAP_SIZE / 2;

    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            mapCells[z][x].A.x = coordX;
            mapCells[z][x].A.z = coordZ;
            
            mapCells[z][x].B.x = coordX;
            mapCells[z][x].B.z = coordZ + 1;

            mapCells[z][x].C.x = coordX + 1;
            mapCells[z][x].C.z = coordZ;

            mapCells[z][x].D.x = coordX + 1;
            mapCells[z][x].D.z = coordZ + 1;

            coordX += 1;
        }
        coordZ += 1;
        coordX = -MAP_SIZE / 2;
    }

    // Bota as coordenadas y nos pontos A's.
    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++){
            mapCells[z][x].A.y = heightMatrix[z][x];
        }
    }

    // Mesma coisa pro B
    for (int z = 0; z < MAP_SIZE - 1; z++) {
        for (int x = 0; x < MAP_SIZE; x++){
            mapCells[z][x].B.y = heightMatrix[z + 1][x];
        }
    }

    // Mesma coisa pro C
    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE - 1; x++){
            mapCells[z][x].C.y = heightMatrix[z][x + 1];
        }
    }

    // Mesma coisa pro D
    for (int z = 0; z < MAP_SIZE - 1; z++) {
        for (int x = 0; x < MAP_SIZE - 1; x++){
            mapCells[z][x].D.y = heightMatrix[z + 1][x + 1];
        }
    }
}

void drawMap() {
    glColor3f(0.2f, 0.8f, 0.2f);

    // Loop para percorrer cada linha do mapa no eixo Z
    for (int z = 0; z < MAP_SIZE; z++) {
        
        glBegin(GL_TRIANGLE_STRIP);

        // Loop para percorrer cada coluna do mapa no eixo X
        for (int x = 0; x <= MAP_SIZE; x++) {
            
            glVertex3f(mapCells[z][x].A.x, mapCells[z][x].A.y, mapCells[z][x].A.z);
            glVertex3f(mapCells[z][x].B.x, mapCells[z][x].B.y, mapCells[z][x].B.z);;
        }

        glEnd();
    }
}