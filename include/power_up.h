#ifndef POWER_UP_H
#define POWER_UP_H

#include <mybib.h>
#include <input.h>

#define MAX_POWER_UPS 256

extern ObjModel snowFlake, hermesShoes, fist, healthPack; //Ammo é carregado em projectile.c

typedef enum {
    PU_AMMO,
    PU_HEALTH,
    PU_SPEED,
    PU_DMG,
    PU_FREEZE
} PowerUpType;

typedef struct {
    PowerUpType type;
    float x, y, z;
    //float scale;
    int active;
} PowerUpInstance;

extern PowerUpInstance powerUps[MAX_POWER_UPS];

PowerUpType getRandomPowerUp();

float getScale(PowerUpType type);

void spawnPowerUp();

void testePowerUp();

void initPowerUps();

// ------------- FREEZE -------------
void drawSnowFlake();

void freeze();

// ------------- AMMO -------------
void drawAmmo();

void addAmmo();

// ------------- HEALTH -------------
void drawHealthPack();

void heal();

// ------------- INSTA KILL -------------
void drawFist();

void increaseDmg();

// ------------- SPEED -------------
void drawHermesShoes();

void increaseSpeed();

// ------------- SHIELD -------------

#endif