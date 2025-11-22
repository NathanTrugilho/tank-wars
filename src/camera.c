#include <camera.h>
#include <input.h> // Precisamos ler keyStates aqui para mover a câmera
#include <stdio.h>

// Inicializa variáveis globais
int freeCameraMode = 0;
float fcX = 0.0f, fcY = 10.0f, fcZ = 0.0f;
float fcAngleH = 0.0f;
float fcAngleV = 0.0f;

void updateCamera()
{
    // Configuração da projeção 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 100); // ratio vem de game.h (extern)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (freeCameraMode) {
        // LÓGICA DA CÂMERA LIVRE 
        
        // Controles de Rotação (Setinhas)
        if (specialKeyStates[GLUT_KEY_LEFT]) fcAngleH += FREE_CAM_ROT_SPEED;
        if (specialKeyStates[GLUT_KEY_RIGHT]) fcAngleH -= FREE_CAM_ROT_SPEED;
        if (specialKeyStates[GLUT_KEY_UP])   fcAngleV += FREE_CAM_ROT_SPEED;
        if (specialKeyStates[GLUT_KEY_DOWN]) fcAngleV -= FREE_CAM_ROT_SPEED;

        // Limita o olhar para cima/baixo
        if (fcAngleV > 89.0f) fcAngleV = 89.0f;
        if (fcAngleV < -89.0f) fcAngleV = -89.0f;

        // Conversão para Radianos
        float radH = fcAngleH * 3.14159f / 180.0f;
        float radV = fcAngleV * 3.14159f / 180.0f;

        // Vetor "Frente" (para onde estou olhando)
        // No OpenGL o Z é 'para fora', então invertemos algumas coisas dependendo da lógica
        float dirX = sinf(radH) * cosf(radV);
        float dirY = sinf(radV);
        float dirZ = -cosf(radH) * cosf(radV); // Z negativo é "para frente" no padrão gluLookAt

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
            fcX += rightX * FREE_CAM_SPEED;
            fcZ += rightZ * FREE_CAM_SPEED;
        }
        if (keyStates['a'] || keyStates['A']) {
            fcX -= rightX * FREE_CAM_SPEED;
            fcZ -= rightZ * FREE_CAM_SPEED;
        }
        
        // Q e E para subir e descer verticalmente absoluto
        if (keyStates['q'] || keyStates['Q']) fcY += FREE_CAM_SPEED;
        if (keyStates['e'] || keyStates['E']) fcY -= FREE_CAM_SPEED;

        // LookAt da Câmera Livre
        gluLookAt(fcX, fcY, fcZ,        // Eye
                  fcX + dirX, fcY + dirY, fcZ + dirZ, // Center (Eye + Vetor Frente)
                  0.0f, 1.0f, 0.0f);    // Up

    } else {
        // LÓGICA DA CÂMERA DO TANQUE
        
        float camX = tankX + sinf(turretAngle * 3.14159 / 180.0) * CAM_FACTOR_X;  
        float camY = CAM_FACTOR_Y - (pipeAngle * 0.1f); 
        float camZ = tankZ + cosf(turretAngle * 3.14159 / 180.0) * CAM_FACTOR_Z;

        if (camY < 1.0f) camY = 1.0f; 
        if (camY > 5.0f) camY = 5.0f;

        gluLookAt(camX, camY, camZ, 
                  tankX, 0.5f, tankZ,
                  0.0f, 1.0f, 0.0f);
    }
    
    drawSun();
}