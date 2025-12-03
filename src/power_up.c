#include <power_up.h>
#include <tank.h>
#include <input.h>

PowerUpInstance powerUps[MAX_POWER_UPS];

//static int powerUpIndex = 0;

// ------------- TESTE --------------
void testePowerUp() {
    if (keyStates['m'] || keyStates['M']) {
        addAmmo();
    }
    if (keyStates['h'] || keyStates['H']) {
        heal();
    }
    if (keyStates['r'] || keyStates['R']) {
        increaseSpeed();
    }
}

// ------------- SPAWN POWER UP -------------

void spawnPowerUp() {
    srand(time(NULL));
    powerUps->x = (rand() % 50) - 25; // Exemplo: posição aleatória entre -25 e 25
    powerUps->y = 1;
    powerUps->z = (rand() % 50) - 25; // Exemplo: posição aleatória entre -25 e 25
}

// ------------- Random Power Up -------------

// ------------- INIT -------------
ObjModel snowFlake, hermesShoes, fist, healthPack; //Ammo é carregado em projectile.c

void initPowerUps() {
    if (loadOBJ("objects/snowFlake.obj", "objects/snowFlake.mtl", &snowFlake)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da neve.\n");
    }
    if (loadOBJ("objects/hermesShoes.obj", "objects/hermesShoes.mtl", &hermesShoes)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da neve.\n");
    }
    if (loadOBJ("objects/fist.obj", "objects/fist.mtl", &fist)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da neve.\n");
    }
    if (loadOBJ("objects/healthPack.obj", "objects/healthPack.mtl", &healthPack)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da neve.\n");
    }
}

// ------------- FREEZE -------------
void drawSnowFlake() {
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

void freeze() {

}

// ------------- AMMO -------------
void drawAmmo() {
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

void addAmmo() {
    player.ammo += 10;
}

// ------------- HEALTH -------------
void drawHealthPack() {
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

void heal() {
    player.health += 30;
}

// ------------- DMG UPGRADE -------------
void drawFist() {
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

void increaseDmg() {
    player.bulletDmg += 15;
}

// ------------- SPEED -------------
void drawHermesShoes() {
    float x = 30, y = 1, z = 13;
    float modelScale = 0.2f;
    
    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(modelScale, modelScale, modelScale);
        //glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        drawModel(&hermesShoes);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void increaseSpeed() {
    player.moveSpeed += 0.1;
    player.tankRotSpeed += 0.1;
    player.turretRotSpeed += 0.15;
}