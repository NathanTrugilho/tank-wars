#include "camera.h"
#include "input.h" // Precisamos ler keyStates aqui para mover a câmera
#include <stdio.h>
#include <math.h>

// Inicializa variáveis globais
int freeCameraMode = 0;

float fcAngleH;
float fcAngleV;
float fcX, fcY, fcZ;

float dirX, dirY, dirZ;

void updateCamera()
{
    // Configuração da projeção 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 200); // ratio vem de game.h (extern)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (freeCameraMode) {
        CalculateFreeCamNewPosition();
        // LookAt da Câmera Livre
        gluLookAt(fcX, fcY, fcZ,        // Eye
                  fcX + dirX, fcY + dirY, fcZ + dirZ, // Center (Eye + Vetor Frente)
                  0.0f, 1.0f, 0.0f);    // Up

    } else {
        // LÓGICA DA CÂMERA DO TANQUE
        float totalAngle = player.hullAngle + player.turretAngle;
        float rad = totalAngle * 3.14159f / 180.0f;

        float camX = player.x + sinf(rad) * CAM_FACTOR_X;
        float camZ = player.z + cosf(rad) * CAM_FACTOR_Z;

        float camY = player.y + CAM_FACTOR_Y - (player.pipeAngle * 0.1f); 
        
        if (camY < player.y + 0.5f) camY = player.y + 0.5f;

        gluLookAt(camX, camY, camZ, 
                  player.x, player.y + 0.5f, player.z, 
                  0.0f, 1.0f, 0.0f);
    }
}

void updateMinimapCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90, 1.0, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Câmera estática no centro do mapa
    gluLookAt(25.0f, 30.0f, 25.0f, 
              25.0f, 0.0f, 25.0f,
              0.0f, 0.0f, -1.0f); 

    drawMapSun();
}

void CalculateFreeCamNewPosition(){
    if (specialKeyStates[GLUT_KEY_LEFT]) fcAngleH -= FREE_CAM_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_RIGHT]) fcAngleH += FREE_CAM_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_UP])   fcAngleV += FREE_CAM_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_DOWN]) fcAngleV -= FREE_CAM_ROT_SPEED;

    // Limita o olhar para cima/baixo
    if (fcAngleV > 89.0f) fcAngleV = 89.0f;
    if (fcAngleV < -89.0f) fcAngleV = -89.0f;

    float radH = fcAngleH * 3.14159f / 180.0f;
    float radV = fcAngleV * 3.14159f / 180.0f;

    // Vetor "Frente" (para onde estou olhando)
    // No OpenGL o Z é 'para fora', então invertemos algumas coisas dependendo da lógica
    dirX = sinf(radH) * cosf(radV);
    dirY = sinf(radV);
    dirZ = -cosf(radH) * cosf(radV);

    // Controles de Movimento (W/A/S/D + Q/E para subir/descer)
    // W/S andam na direção que se olha (exceto Y, para andar no plano como FPS, ou full 3D)

    if (keyStates['w'] || keyStates['W']) {
        fcX += dirX * FREE_CAM_SPEED;
        fcY += dirY * FREE_CAM_SPEED;
        fcZ += dirZ * FREE_CAM_SPEED;
    }
    if (keyStates['s'] || keyStates['S']) {
        fcX -= dirX * FREE_CAM_SPEED;
        fcY -= dirY * FREE_CAM_SPEED;
        fcZ -= dirZ * FREE_CAM_SPEED;
    }

    // Strafe (andar de lado) - Vetor perpendicular
    float rightX = sinf(radH - 3.14159f/2.0f);
    float rightZ = -cosf(radH - 3.14159f/2.0f);

    if (keyStates['d'] || keyStates['D']) {
        fcX -= rightX * FREE_CAM_SPEED;
        fcZ -= rightZ * FREE_CAM_SPEED;
    }
    if (keyStates['a'] || keyStates['A']) {
        fcX += rightX * FREE_CAM_SPEED;
        fcZ += rightZ * FREE_CAM_SPEED;
    }
    // Q e E para subir e descer verticalmente absoluto
    if (keyStates['q'] || keyStates['Q']) fcY -= FREE_CAM_SPEED;
    if (keyStates['e'] || keyStates['E']) fcY += FREE_CAM_SPEED;
}