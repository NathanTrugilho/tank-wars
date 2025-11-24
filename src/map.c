#include <map.h>
#include <stdio.h>
#include <mybib.h>

square mapCells[MAP_SIZE][MAP_SIZE];

float heightMatrix[VERTEX_NUM][VERTEX_NUM];

vertex vertexNormals[VERTEX_NUM][VERTEX_NUM];

ObjModel churchModel;

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

void initChurch() {
    churchModel.faceCount = 0; 
    loadOBJ("objects/igreja.obj", "objects/igreja.mtl", &churchModel);
}

void initMapCells(){

    initHeightMatrix();

    float coordZ = 0;
    float coordX = 0;

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
        coordX = 0;
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
    glColor3f(0.1f, 0.6f, 0.1f);

    for (int z = 0; z < MAP_SIZE; z++) {
        
        for (int x = 0; x < MAP_SIZE; x++) {
            
            glBegin(GL_TRIANGLE_STRIP);

            vertex normalA = vertexNormals[z][x];
            glNormal3f(normalA.x, normalA.y, normalA.z);
            glVertex3f(mapCells[z][x].A.x, mapCells[z][x].A.y, mapCells[z][x].A.z);
        
            vertex normalB = vertexNormals[z+1][x];
            glNormal3f(normalB.x, normalB.y, normalB.z);
            glVertex3f(mapCells[z][x].B.x, mapCells[z][x].B.y, mapCells[z][x].B.z);

            vertex normalC = vertexNormals[z][x+1];
            glNormal3f(normalC.x, normalC.y, normalC.z);
            glVertex3f(mapCells[z][x].C.x, mapCells[z][x].C.y, mapCells[z][x].C.z);

            vertex normalD = vertexNormals[z+1][x+1];
            glNormal3f(normalD.x, normalD.y, normalD.z);
            glVertex3f(mapCells[z][x].D.x, mapCells[z][x].D.y, mapCells[z][x].D.z);

            glEnd();
        }
    }

    glEnable(GL_TEXTURE_2D); 
    glPushMatrix();
        
        int cellX = (int)CHURCH_X;
        int cellZ = (int)CHURCH_Z;
        
        float churchY = mapCells[cellZ][cellX].A.y; 

        glTranslatef(CHURCH_X, churchY, CHURCH_Z); 
        
        float debugScale = 1.0f; 
        glScalef(debugScale, debugScale, debugScale);

        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        
        if (churchModel.faceCount > 0) {
            drawModel(&churchModel);
        }
        
    glPopMatrix();
}