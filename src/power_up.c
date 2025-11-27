#include <power_up.h>
#include <tank.h>
#include <input.h>

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

// ------------- INIT -------------
ObjModel snowFlake;

void initPowerUps() {
    if (loadOBJ("objects/SnowflakePintado.obj", "objects/snowflake.mtl", &snowFlake)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da neve.\n");
    }
}

// ------------- FREEZE -------------
void drawSnowFlake() {
    float modelScale = 0.2f;
    float estimatedModelRadius = 5.0f; // Raio aproximado do seu modelo em unidades locais
    float worldRadius = estimatedModelRadius * modelScale;
    
    // Modelo
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(20, 1, 40);
        glScalef(modelScale, modelScale, modelScale);
        drawModel(&snowFlake);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    // Esfera com raio calculado
    glPushMatrix();
        glTranslatef(20, 1, 40);
        glColor3f(0.0f, 1.0f, 1.0f); // Ciano
        glutWireSphere(worldRadius, 16, 12);
    glPopMatrix();
}

void freeze() {

}

// ------------- AMMO -------------
void drawAmmo() {

}

void addAmmo() {
    player.ammo += 10;
}

// ------------- HEALTH -------------
void drawHealthPack() {

}

void heal() {
    player.health += 30;
}

// ------------- INSTA KILL -------------
void drawSkull() {

}

void increaseDmg() {
    player.bulletDmg += 15;
}

// ------------- SPEED -------------
void drawMarioStar() {
    
}

void increaseSpeed() {
    player.moveSpeed += 0.1;
    player.tankRotSpeed += 0.1;
    player.turretRotSpeed += 0.15;
}

// ------------- SHIELD -------------