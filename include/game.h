#ifndef GAME_H 
#define GAME_H 

#include <GL/glut.h>
#include <tank.h>
#include <enemy.h>
#include <projectile.h>
#include <math.h>
#include <map.h>
#include <camera.h>
#include <lighting.h>

void display();

// Reshape
void reshape(int w, int h);

// Animação
void idle();

// Inicialização
void init();

#endif