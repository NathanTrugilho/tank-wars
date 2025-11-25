#include <map.h>
#include <stdio.h>
#include <mybib.h>

square mapCells[MAP_SIZE][MAP_SIZE];

float heightMatrix[VERTEX_NUM][VERTEX_NUM];

vertex vertexNormals[VERTEX_NUM][VERTEX_NUM];

ObjModel churchModel;
ObjModel houseModel;
ObjModel gasStationModel;
ObjModel storeModel;


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

void initHouse() {
    houseModel.faceCount = 0; 
    loadOBJ("objects/house.obj", "objects/house.mtl", &houseModel);
}

void initGasStation() {
    gasStationModel.faceCount = 0; 
    loadOBJ("objects/gasStation.obj", "objects/gasStation.mtl", &gasStationModel);
}

void initStore() {
    storeModel.faceCount = 0; 
    loadOBJ("objects/store.obj", "objects/store.mtl", &storeModel);
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

    // ===================================
    // Draw Church
    // ===================================
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


    // ===================================
    // Draw House
    // ===================================
    glPushMatrix();
        int houseCellX = (int)HOUSE_X;
        int houseCellZ = (int)HOUSE_Z;
        float houseY = mapCells[houseCellZ][houseCellX].A.y; 

        glTranslatef(HOUSE_X, houseY, HOUSE_Z); 

        float houseScale = 1.0f; 
        glScalef(houseScale, houseScale, houseScale);
        // glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 

        if (houseModel.faceCount > 0) {
            drawModel(&houseModel);
        }
        
    glPopMatrix();

    glPushMatrix();
        int house2CellX = (int)HOUSE2_X;
        int house2CellZ = (int)HOUSE2_Z;
        float house2Y = mapCells[house2CellZ][house2CellX].A.y; 

        glTranslatef(HOUSE2_X, house2Y, HOUSE2_Z); 

        float house2Scale = 1.0f; 
        glScalef(house2Scale, house2Scale, house2Scale);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 

        if (houseModel.faceCount > 0) {
            drawModel(&houseModel);
        }
        
    glPopMatrix();

    // ===================================
    // Draw Gas Station
    // ===================================
    glPushMatrix();
        int gasStationCellX = (int)GASSTATION_X;
        int gasStationCellZ = (int)GASSTATION_Z;
        float gasStationY = mapCells[gasStationCellZ][gasStationCellX].A.y; 

        glTranslatef(GASSTATION_X, gasStationY, GASSTATION_Z); 

        float gasStationScale = 0.7f; 
        glScalef(gasStationScale, gasStationScale, gasStationScale);
        // glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 

        if (gasStationModel.faceCount > 0) {
            drawModel(&gasStationModel);
        }
        
    glPopMatrix();

    glPushMatrix();
        int gasStation2CellX = (int)GASSTATION2_X;
        int gasStation2CellZ = (int)GASSTATION2_Z;
        float gasStation2Y = mapCells[gasStation2CellZ][gasStation2CellX].A.y; 

        glTranslatef(GASSTATION2_X, gasStation2Y, GASSTATION2_Z); 

        float gasStation2Scale = 0.7f; 
        glScalef(gasStation2Scale, gasStation2Scale, gasStation2Scale);
        // glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 

        if (gasStationModel.faceCount > 0) {
            drawModel(&gasStationModel);
        }
        
    glPopMatrix();

    glPushMatrix();
        int gasStation3CellX = (int)GASSTATION3_X;
        int gasStation3CellZ = (int)GASSTATION3_Z;
        float gasStation3Y = mapCells[gasStation3CellZ][gasStation3CellX].A.y; 

        glTranslatef(GASSTATION3_X, gasStation3Y, GASSTATION3_Z); 

        float gasStation3Scale = 0.7f; 
        glScalef(gasStation3Scale, gasStation3Scale, gasStation3Scale);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 

        if (gasStationModel.faceCount > 0) {
            drawModel(&gasStationModel);
        }
        
    glPopMatrix();

    glPushMatrix();
        int gasStation4CellX = (int)GASSTATION4_X;
        int gasStation4CellZ = (int)GASSTATION4_Z;
        float gasStation4Y = mapCells[gasStation4CellZ][gasStation4CellX].A.y; 

        glTranslatef(GASSTATION4_X, gasStation4Y, GASSTATION4_Z); 

        float gasStation4Scale = 0.7f; 
        glScalef(gasStation4Scale, gasStation4Scale, gasStation4Scale);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 

        if (gasStationModel.faceCount > 0) {
            drawModel(&gasStationModel);
        }
        
    glPopMatrix();

    // ===================================
    // Draw Store
    // ===================================
    glPushMatrix();
        int storeCellX = (int)STORE_X;
        int storeCellZ = (int)STORE_Z;
        float storeY = mapCells[storeCellZ][storeCellX].A.y; 

        glTranslatef(STORE_X, storeY, STORE_Z); 

        float storeScale = 1.0f; 
        glScalef(storeScale, storeScale, storeScale);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 

        if (storeModel.faceCount > 0) {
            drawModel(&storeModel);
        }
        
    glPopMatrix();
}