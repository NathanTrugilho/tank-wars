#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include <tank.h> // Para pegar tankX, tankZ quando alternar
#include <math.h>
#include <game.h>
// #include <game.h> // Evitar include circular se possível, mas ok se necessário

// Constantes da Câmera do Tanque (Mantive as suas)
#define CAM_FACTOR_X 5.0f
#define CAM_FACTOR_Y 2.7f
#define CAM_FACTOR_Z 5.0f

// Constantes da Câmera Livre
#define FREE_CAM_SPEED 0.5f
#define FREE_CAM_ROT_SPEED 2.0f

// Variável de Estado (0 = Tanque, 1 = Livre)
extern int freeCameraMode;

extern float fcAngleH;
extern float fcAngleV;
extern float fcX, fcY, fcZ;

void updateCamera();

void updateMinimapCamera();

void drawMapSun();

void CalculateFreeCamNewPosition();

#endif


