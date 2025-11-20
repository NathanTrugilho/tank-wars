#ifndef TANK_H
#define TANK_H

#include <GL/glut.h>
#include <projectile.h>
#include <input.h>
#include <map.h>
#include <stdio.h>
#include <mybib.h>
#include <utils.h>
#include <collision.h>

#define INITIAL_TANK_CELL_Z 25
#define INITIAL_TANK_CELL_X 25

#define MAX_PIPE_ANGLE 10
#define MIN_PIPE_ANGLE -5

#define TANK_MOVEMENT_SPEED 0.1

#define TANK_ROT_SPEED 1.0f
#define TURRET_ROT_SPEED 0.5f
#define PIPE_INCLINE_SPEED 0.1f

// Defini isso na mão pq não tenho como saber exatamente onde está o pipe
#define PIPE_LENGTH 3.2
#define PIPE_HEIGHT 1.12

#define TRUE 1
#define FALSE 0

extern const double RADIAN_FACTOR; // Pra converter de graus pra radiano

// Posição e orientação do tanque
extern float tankX;
extern float tankY;
extern float tankZ;

extern float hullAngle;
extern float turretAngle;
extern float pipeAngle;

extern int mapCellX;
extern int mapCellZ;

extern ObjModel turretModel, pipeModel, hullModel;

void drawTank();

void updateTank();

void initTank();

void updateMapCellPos();

#endif