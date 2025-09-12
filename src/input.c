#include <input.h>

// Teclas
int keyStates[256] = {0};

// Teclado
void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = 1;
    if (key == ' ') shootBullet();
}
void keyUp(unsigned char key, int x, int y) { keyStates[key] = 0; }