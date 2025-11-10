#include <GL/glut.h>
#include <projectile.h>
#include <input.h>
#include <map.h>
#include <stdio.h>
#include <mybib.h>

#define INITIAL_TANK_CELL_Z 25
#define INITIAL_TANK_CELL_X 25

#define MAX_PIPE_ANGLE 5
#define MIN_PIPE_ANGLE -2

// Posição e orientação do tanque
extern float tankX;
extern float tankY;
extern float tankZ;

extern float hullAngle;
extern float turretAngle;
extern float pipeAngle;

extern float moveSpeed;

extern float rotSpeed;
extern float turretRotSpeed;
extern float pipeInclSpeed;

extern int mapCellX;
extern int mapCellZ;

extern ObjModel turretModel, pipeModel, hullModel;

void drawTank();

void updateTank();

void initTank();

void updateMapCellPos();