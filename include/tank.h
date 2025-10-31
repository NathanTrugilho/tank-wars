#ifndef TANK_H
#define TANK_H

#include <GL/glut.h>
#include <projectile.h>
#include <input.h>
#include <map.h>
#include <stdio.h>
#include <collision.h>

#define INITIAL_TANK_CELL_Z 25
#define INITIAL_TANK_CELL_X 25

typedef struct {
    Vector3f position;
    float tankAngle;
    float moveSpeed;
    float rotSpeed;
    int mapCellX;
    int mapCellZ;
    int alive;
    AABB hitbox;
} Tank;

extern Tank player;

void drawTank();

void updateTank();

void initTank();

void updateMapCellPos();

AABB getTankAABB(Vector3f position);

int checkPlayerCollision(AABB testBox);

#endif