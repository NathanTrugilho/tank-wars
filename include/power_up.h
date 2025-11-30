#ifndef POWER_UP_H
#define POWER_UP_H

#include <mybib.h>
#include <input.h>

extern ObjModel snowFlake;

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