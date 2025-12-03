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

// ------------- GET RANDOM POWER UP -------------

PowerUpType getRandomPowerUp()
{
    int r = rand() % 5; // cinco tipos no enum
    return (PowerUpType)r;
}

// ------------- GET SCALE -------------
float getScale(PowerUpType type)
{
    switch (type)
    {
    case PU_AMMO:
        return 0.35f;
        break;

    case PU_HEALTH:
        return 0.09f;
        break;

    case PU_SPEED:
        return 0.2f;
        break;

    case PU_DMG:
        return 0.1f;
        break;

    case PU_FREEZE:
        return 0.2f;
        break;

    default:
        break;
    }
}

// ------------- SPAWN POWER UP -------------

void spawnPowerUp()
{
    float now = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    if (now - lastSpawnTime >= 3.0f)
    { // 10 segundos
        printf("%d", powerUps[lastPowerUpIndex].type);
        lastSpawnTime = now;
        switch (powerUps[lastPowerUpIndex].type)
        {
        case PU_AMMO:
            drawAmmo();
            break;

        case PU_HEALTH:
            drawHealthPack();
            break;

        case PU_SPEED:
            drawHermesShoes();
            break;

        case PU_DMG:
            drawFist();
            break;

        case PU_FREEZE:
            drawSnowFlake();
            break;

        default:
            break;
        }
    }
    lastPowerUpIndex += 1;
}

// ------------- INIT -------------
ObjModel snowFlake, hermesShoes, fist, healthPack; // Ammo é carregado em projectile.c

void initPowerUps()
{
    if (loadOBJ("objects/snowFlake.obj", "objects/snowFlake.mtl", &snowFlake))
   
    if (loadOBJ("objects/hermesShoes.obj", "objects/hermesShoes.mtl", &hermesShoes))
    
    if (loadOBJ("objects/fist.obj", "objects/fist.mtl", &fist))
    
    if (loadOBJ("objects/healthPack.obj", "objects/healthPack.mtl", &healthPack));
    //srand(time(NULL))
    for (int i = 0; i < MAX_POWER_UPS; i++)
    {
        powerUps->type = getRandomPowerUp();
        powerUps->x = (rand() % 50) - 25;
        powerUps->y = 1;
        powerUps->z = (rand() % 50) - 25;
        //powerUps->scale = getScale(powerUps->type);
        powerUps->active = 0;
    }
}

// ------------- FREEZE -------------
void drawSnowFlake()
{
    float x = 30, y = 1, z = 13;
    float modelScale = 0.2f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
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
void drawAmmo()
{
    float x = 30, y = 1, z = 13;
    float modelScale = 0.35f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
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
void drawHealthPack()
{
    float x = 30, y = 1, z = 13;
    float modelScale = 0.09f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
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
void drawFist()
{
    float x = 30, y = 1, z = 13;
    float modelScale = 0.1f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
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
void drawHermesShoes()
{
    float x = 30, y = 1, z = 13;
    float modelScale = 0.2f;

    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
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