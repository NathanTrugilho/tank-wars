#ifndef ENEMY_H
#define ENEMY_H


#include "mybib.h" // Para ter acesso ao tipo ObjModel


#define MAX_ENEMIES 10


typedef struct {
int alive;
float x, z;
float hullAngle; // Adicionado para rotação da base do inimigo
float turretAngle; // Adicionado para rotação da torre do inimigo
} Enemy;


extern Enemy enemies[MAX_ENEMIES];


// Modelos 3D compartilhados (agora expostos como externs para colisão)
extern ObjModel enemyHullModel;
extern ObjModel enemyTurretModel;
extern ObjModel enemyPipeModel;


// Novas funções expostas
void initEnemies();
void updateEnemies(float playerX, float playerZ); // Opcional: passar pos do player pra eles mirarem
void drawEnemies();


#endif