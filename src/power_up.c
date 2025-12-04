#include <power_up.h>
#include <tank.h>
#include <input.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

PowerUpInstance powerUps[MAX_POWER_UPS];

static int lastPowerUpIndex = 0;
static float lastSpawnTime = 0.0f;

// ------------- TESTE --------------
void testePowerUp()
{
    if (keyStates['m'] || keyStates['M'])
    {
        addAmmo();
    }
    if (keyStates['h'] || keyStates['H'])
    {
        heal();
    }
    if (keyStates['r'] || keyStates['R'])
    {
        increaseSpeed();
    }
}

// ------------- GET SCALE -------------
float getScale(PowerUpType type)
{
    switch (type)
    {
    case PU_AMMO:
        return 0.35f;

    case PU_HEALTH:
        return 0.09f;

    case PU_SPEED:
        return 0.2f;

    case PU_DMG:
        return 0.1f;

    case PU_FREEZE:
        return 0.2f;
    }

    return 0.1f;
}

// ------------- GET RANDOM POWER UP -------------

PowerUpType getRandomPowerUp()
{
    int r = rand() % 5; // cinco tipos no enum
    return (PowerUpType)r;
}

// ------------- APPLY EFEITO DO POWER UP -------------

void applyPowerUpEffect(PowerUpType type)
{
    switch (type)
    {
        case PU_AMMO:
            addAmmo();
            break;

        case PU_HEALTH:
            heal();
            break;

        case PU_SPEED:
            increaseSpeed();
            break;

        case PU_DMG:
            increaseDmg();
            break;

        case PU_FREEZE:
            freeze();
            break;
    }
}


// ------------- SPAWN POWER UP -------------

void spawnPowerUp()
{
    float now = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    if (now - lastSpawnTime >= 5.0f)
    { // 5 segundos
        lastSpawnTime = now;
        powerUps[lastPowerUpIndex].active = 1;
        lastPowerUpIndex += 1;
    }
}

// ------------- DRAW ACTIVE POWER UPS -------------
void drawPowerUps()
{
    for (int i = 0; i < MAX_POWER_UPS; i++)
    {
        if (!powerUps[i].active)
            continue;

        switch (powerUps[i].type)
        {
        case PU_AMMO:
            drawAmmo(i);
            break;
        case PU_HEALTH:
            drawHealthPack(i);
            break;
        case PU_SPEED:
            drawHermesShoes(i);
            break;
        case PU_DMG:
            drawFist(i);
            break;
        case PU_FREEZE:
            drawSnowFlake(i);
            break;
        }
    }
}

// ------------- INIT -------------
ObjModel snowFlake, hermesShoes, fist, healthPack; // Ammo é carregado em projectile.c

void initPowerUps()
{
    if (loadOBJ("objects/snowFlake.obj", "objects/snowFlake.mtl", &snowFlake)) {}
    else {}

    if (loadOBJ("objects/hermesShoes.obj", "objects/hermesShoes.mtl", &hermesShoes)){}
    else {}

    if (loadOBJ("objects/fist.obj", "objects/fist.mtl", &fist)){}
    else {}

    if (loadOBJ("objects/healthPack.obj", "objects/healthPack.mtl", &healthPack)){}
    else {}

    for (int i = 0; i < MAX_POWER_UPS; i++)
    {
        powerUps[i].type = getRandomPowerUp();
        powerUps[i].x = (rand() % 50) - 25;
        powerUps[i].y = 1;
        powerUps[i].z = (rand() % 50) - 25;
        powerUps[i].scale = getScale(powerUps[i].type);
        powerUps[i].active = 0;
    }
}

// ------------- GET OBJECT MODEL -------------

ObjModel getObjModel(PowerUpInstance p)
{
    switch (p.type)
    {
    case PU_AMMO:
        return shellModel;
    case PU_HEALTH:
        return healthPack;
    case PU_SPEED:
        return hermesShoes;
    case PU_DMG:
        return fist;
    case PU_FREEZE:
        return snowFlake;
    }

    return shellModel;
}

// ------------- FREEZE -------------
void drawSnowFlake(int index)
{
    float modelScale = 0.2f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(powerUps[index].x, powerUps[index].y, powerUps[index].z);
    glScalef(modelScale, modelScale, modelScale);
    drawModel(&snowFlake);
    drawBox(snowFlake.box);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void freeze()
{
}

// ------------- AMMO -------------
void drawAmmo(int index)
{
    float modelScale = 0.35f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(powerUps[index].x, powerUps[index].y, powerUps[index].z);
    glScalef(modelScale, modelScale, modelScale);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawModel(&shellModel);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void addAmmo()
{
    player.ammo += 10;
}

// ------------- HEALTH -------------
void drawHealthPack(int index)
{
    float modelScale = 0.09f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(powerUps[index].x, powerUps[index].y, powerUps[index].z);
    glScalef(modelScale, modelScale, modelScale);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawModel(&healthPack);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void heal()
{
    player.health += 30;
}

// ------------- DMG UPGRADE -------------
void drawFist(int index)
{
    float modelScale = 0.1f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(powerUps[index].x, powerUps[index].y, powerUps[index].z);
    glScalef(modelScale, modelScale, modelScale);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    drawModel(&fist);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void increaseDmg()
{
    player.bulletDmg += 15;
}

// ------------- SPEED -------------
void drawHermesShoes(int index)
{
    float modelScale = 0.2f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(powerUps[index].x, powerUps[index].y, powerUps[index].z);
    glScalef(modelScale, modelScale, modelScale);
    // glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawModel(&hermesShoes);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void increaseSpeed()
{
    player.moveSpeed += 0.1;
    player.tankRotSpeed += 0.1;
    player.turretRotSpeed += 0.15;
}