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
    if (loadOBJ("objects/snowflake.obj", "objects/snowflake.mtl", &snowFlake)) {
    } else {
        printf("ERRO: Nao foi possivel carregar o modelo da neve.\n");
    }
}

// ------------- FREEZE -------------
void drawSnowFlake() {
    
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