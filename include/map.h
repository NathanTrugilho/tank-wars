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

void drawMap();

#endif