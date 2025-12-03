#include <input.h>
#include <camera.h> // Para acessar freeCameraMode e atualizar posições
#include <tank.h>   // Para pegar posição do tanque ao trocar

// Teclas
int keyStates[256] = {0};
int specialKeyStates[256] = {0};

// Teclado
void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = 1;
    if (key == ' ' && player.ammo > 0) shootBullet();
    
    if (key == ' ') shootBullet();

    // Alternar Câmera (L ou l)
    if (key == 'l' || key == 'L') {
        freeCameraMode = !freeCameraMode; // Inverte 0 <-> 1

        if (freeCameraMode) {
            // Ao entrar no modo livre, teletransporta a câmera para cima do tanque
            // para a transição ser suave
            fcX = tankX;
            fcY = tankY + 5.0f; // Um pouco acima
            fcZ = tankZ + 5.0f; // Um pouco atrás
            fcAngleH = 180.0f;  // Olhando pra trás 
            fcAngleV = -20.0f;  // Olhando levemente para baixo
        }
    }
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