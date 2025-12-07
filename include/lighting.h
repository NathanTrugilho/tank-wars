#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/glut.h>

extern long freezeEndTime;

void drawSun();

void drawMapSun();

void setupLighting();

void updateFreezeLighting();

#endif