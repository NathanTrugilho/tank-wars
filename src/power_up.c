#include <power_up.h>
#include <tank.h>

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

// ------------- AMMO -------------
void drawAmmo() {

}

// ------------- HEALTH -------------
void drawHealthPack() {

}

// ------------- INSTA KILL -------------
void drawSkull() {

}

// ------------- SPEED -------------
void drawStar() {
    
}