#ifndef MAP_H
#define MAP_H

#include <GL/glut.h>

#define MAP_SIZE 100

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

extern float heightMatrix[MAP_SIZE][MAP_SIZE];

extern vertex vertexNormals[MAP_SIZE][MAP_SIZE];

void drawMap();

void initHeightMatrix();

void initMapCells();

#endif