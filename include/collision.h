#ifndef COLLISION_H
#define COLLISION_H

typedef struct {
    float x, y, z;
} Vector3f;

typedef struct {
    Vector3f min;
    Vector3f max;
} AABB;

int checkCollisionAABB(AABB box1, AABB box2);

#endif