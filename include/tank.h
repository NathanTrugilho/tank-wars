#include <GL/glut.h>
#include <projectile.h>
#include <input.h>
#include <map.h>
#include <stdio.h>

#define INITIAL_TANK_CELL_Z 25
#define INITIAL_TANK_CELL_X 25

// Posição e orientação do tanque
extern float tankX;
extern float tankY;
extern float tankZ;

extern float tankAngle;
extern float moveSpeed;
extern float rotSpeed;

extern int mapCellX;
extern int mapCellZ;

void drawTank();

void updateTank();

void initTank();

void updateMapCellPos();