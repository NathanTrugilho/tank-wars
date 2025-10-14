#ifndef MAP_H
#define MAP_H

#include <GL/glut.h>

#define MAP_SIZE 50
#define VERTEX_NUM MAP_SIZE + 1 // Tem que ser um a mais que o tamanho do mapa

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

extern square mapCells[MAP_SIZE][MAP_SIZE]; // A --- C
                                            // |     |
                                            // B --- D



extern float heightMatrix[VERTEX_NUM][VERTEX_NUM];

extern vertex vertexNormals[VERTEX_NUM][VERTEX_NUM];

void drawMap();

void initHeightMatrix();

void initMapCells();

#endif