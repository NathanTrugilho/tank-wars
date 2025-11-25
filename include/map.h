#ifndef MAP_H
#define MAP_H

#include <GL/glut.h>

#define MAP_SIZE 50
#define VERTEX_NUM MAP_SIZE + 1 // Tem que ser um a mais que o tamanho do mapa

#define CHURCH_X 10.0f
#define CHURCH_Z 18.0f

#define HOUSE_X 15.0f  
#define HOUSE_Z 18.0f

#define HOUSE2_X 19.0f  
#define HOUSE2_Z 9.0f

#define GASSTATION_X 25.0f  
#define GASSTATION_Z 18.0f

#define GASSTATION2_X 30.0f  
#define GASSTATION2_Z 18.0f

#define GASSTATION3_X 25.0f  
#define GASSTATION3_Z 10.0f

#define GASSTATION4_X 30.0f  
#define GASSTATION4_Z 10.0f

#define STORE_X 38.0f  
#define STORE_Z 18.0f

typedef struct vertex {
    float x;
    float y;
    float z;
} vertex;

typedef struct square {
    vertex A;
    vertex B;
    vertex C;
    vertex D;
} square;

// A --- C
// |     |
// B --- D

extern square mapCells[MAP_SIZE][MAP_SIZE]; // [z][x]

extern float heightMatrix[VERTEX_NUM][VERTEX_NUM];

extern vertex vertexNormals[VERTEX_NUM][VERTEX_NUM];

void drawMap();

void initHeightMatrix();

void initMapCells();

void initChurch();

void initHouse();

void initGasStation();

void initStore();

#endif