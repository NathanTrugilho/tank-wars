#include <map.h>
//#include <stdio.h>

square mapCells[MAP_SIZE][MAP_SIZE];

float heightMatrix[VERTEX_NUM][VERTEX_NUM];

vertex vertexNormals[VERTEX_NUM][VERTEX_NUM];

void initHeightMatrix(){

    for (int z = 0; z < VERTEX_NUM; z++) {
        for (int x = 0; x < VERTEX_NUM; x++) {
            heightMatrix[z][x] = 0;
        }
    }

    // Simulando alguma altura
    for (int x = 0; x < VERTEX_NUM; x++) {
        heightMatrix[0][x] = 5.0f;
    }

    for (int x = 0; x < VERTEX_NUM; x++) {
        heightMatrix[50][x] = 5.0f;
    }

    for (int z = 0; z < VERTEX_NUM; z++) {
        heightMatrix[z][0] = 5.0f;
    }

    for (int z = 0; z < VERTEX_NUM; z++) {
        heightMatrix[z][50] = 5.0f;
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
    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++){
            mapCells[z][x].B.y = heightMatrix[z + 1][x];
        }
    }

    // Mesma coisa pro C
    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++){
            mapCells[z][x].C.y = heightMatrix[z][x + 1];
        }
    }

    // Mesma coisa pro D
    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++){
            mapCells[z][x].D.y = heightMatrix[z + 1][x + 1];
        }
    }
}

void drawMap() {
    glColor3f(0.2f, 0.8f, 0.2f);

    // Loop para percorrer cada linha do mapa no eixo Z
    for (int z = 0; z < MAP_SIZE; z++) {
        
        // Loop para percorrer cada coluna do mapa no eixo X
        for (int x = 0; x < MAP_SIZE; x++) {
            
            glBegin(GL_TRIANGLE_STRIP);

            // Vértice A
            vertex normalA = vertexNormals[z][x];
            glNormal3f(normalA.x, normalA.y, normalA.z);
            glVertex3f(mapCells[z][x].A.x, mapCells[z][x].A.y, mapCells[z][x].A.z);
            //printf("A: %f, %f, %f \n ", normalA.x, normalA.y, normalA.z);
        
            // Vértice B
            vertex normalB = vertexNormals[z+1][x];
            glNormal3f(normalB.x, normalB.y, normalB.z);
            glVertex3f(mapCells[z][x].B.x, mapCells[z][x].B.y, mapCells[z][x].B.z);
            //printf("B: %f, %f, %f \n ", normalB.x, normalB.y, normalB.z);

            // Vértice C
            vertex normalC = vertexNormals[z][x+1];
            glNormal3f(normalC.x, normalC.y, normalC.z);
            glVertex3f(mapCells[z][x].C.x, mapCells[z][x].C.y, mapCells[z][x].C.z);
            //printf("C: %f, %f, %f \n ", normalC.x, normalC.y, normalC.z);

            // Vértice D
            vertex normalD = vertexNormals[z+1][x+1];
            glNormal3f(normalD.x, normalD.y, normalD.z);
            glVertex3f(mapCells[z][x].D.x, mapCells[z][x].D.y, mapCells[z][x].D.z);
            //printf("D: %f, %f, %f \n ", normalD.x, normalD.y, normalD.z);

            glEnd();
        }
    }
}