#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    Vector3 position;
    Vector3 size; // largura, altura, profundidade
} Box;

typedef struct {
    Vector3 center;
    Vector3 axes[3];   // vetores unitários locais (X, Y, Z)
    Vector3 halfSize;  // metade do tamanho em cada direção
} OBB; // Oriented Bounding Box

// boolean
bool checkCollisionOBB(OBB a, OBB b);
    // testa 15 eixos possíveis (3 de cada box e 9 cruzamentos
    // se algum eixo separa as projeções, retorna false
    // caso contrário, colisão detectada

#endif // COLLISION_H