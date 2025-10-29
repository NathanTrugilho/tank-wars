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

typedef struct Tank{
    Box hitbox;
    float tankAngle;
    float moveSpeed;
    float rotSpeed;
    int mapCellX;
    int mapCellY;
    int mapCellZ;
} Tank;

extern Tank playerTank;

void initTank(Tank *tank);

void updateTank(Tank *tank);

void drawTank(const Tank *tank);

void updateMapCellPos(Tank *tank);

#endif