#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include <tank.h>
#include <math.h>

void updateCamera();
int wouldCollideTank(float nextX, float nextZ, float hullAngleDeg);

#endif


