#include <input.h>

// Teclas
int keyStates[256] = {0};

int specialKeyStates[256] = {0};

// Teclado
void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = 1;
    if (key == ' ') shootBullet();
}
void keyUp(unsigned char key, int x, int y) { 
    keyStates[key] = 0; 
}

// Teclas Especiais
void specialKeyDown(int key, int x, int y) {
    specialKeyStates[key] = 1;
}

void specialKeyUp(int key, int x, int y) {
    specialKeyStates[key] = 0;
}