#ifndef COLISION_H
#define COLISION_H

#include "mybib.h" // Apenas para ter acesso ao struct Box e ObjModel

// Estrutura auxiliar para um ponto 2D (já que colisão de tanque é no chão X/Z)
typedef struct {
    float x, z;
} Point2D;

// Estrutura que representa um retângulo rotacionado no mundo (4 vértices)
typedef struct {
    Point2D p[4];
} RotatedRect;

// Função principal de colisão
int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg);

#endif