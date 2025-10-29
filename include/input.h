#ifndef INPUT_H
#define INPUT_H

#include <projectile.h>

extern int keyStates[256];

void keyDown(unsigned char key, int x, int y);

void keyUp(unsigned char key, int x, int y);

#endif