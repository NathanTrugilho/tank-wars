#include <collision.h>

int checkCollisionAABB(AABB box1, AABB box2) {
    // Verifica sobreposição no eixo X
    int overlapX = (box1.min.x <= box2.max.x) && (box1.max.x >= box2.min.x);

    // Verifica sobreposição no eixo Y
    int overlapY = (box1.min.y <= box2.max.y) && (box1.max.y >= box2.min.y);

    // Verifica sobreposição no eixo Z
    int overlapZ = (box1.min.z <= box2.max.z) && (box1.max.z >= box2.min.z);

    // Colisão ocorre se houver sobreposição em todos os eixos
    return overlapX && overlapY && overlapZ;
}