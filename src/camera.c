#include "camera.h"
#include "input.h" 
#include <stdio.h>
#include <math.h>

// Inicializa variáveis globais
int freeCameraMode = FALSE;
int currentCameraMode = CAM_THIRD_PERSON;

float fcAngleH;
float fcAngleV;
float fcX, fcY, fcZ;

float dirX, dirY, dirZ;

void updateCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 200); // ratio vem de game.h (extern)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Prioridade: Se o modo "Câmera Livre" (Debug) estiver ligado, ignora os outros
    if (freeCameraMode) {
        CalculateFreeCamNewPosition();
        gluLookAt(fcX, fcY, fcZ,
                  fcX + dirX, fcY + dirY, fcZ + dirZ,
                  0.0f, 1.0f, 0.0f);
        return; // Sai da função, não faz o resto
    }

    // ====================================================================
    // CÁLCULO DO ÂNGULO REAL
    // A direção real para onde olhamos é a soma da rotação do Tanque (Hull)
    // mais a rotação da Torre (Turret).
    // ====================================================================
    float totalAngle = player.hullAngle + player.turretAngle;

    // Lógica dos 3 Modos de Câmera de Jogo
    switch (currentCameraMode) {

        case CAM_FIRST_PERSON: 
        {
            // Alterado: Usamos totalAngle em vez de player.turretAngle
            float h_rad = totalAngle * RADIAN_FACTOR; 
            float v_rad = player.pipeAngle * RADIAN_FACTOR;

            float cameraDist = PIPE_LENGTH + 0.8f; 

            float horizontal_dist = cosf(v_rad) * cameraDist;
            
            float right_x = cosf(h_rad);
            float right_z = -sinf(h_rad);

            // Calcula a posição do olho
            float eyeX = player.x - sinf(h_rad) * horizontal_dist - (right_x * BULLET_SIDE_CORRECTION);
            float eyeZ = player.z - cosf(h_rad) * horizontal_dist - (right_z * BULLET_SIDE_CORRECTION);
            float eyeY = (player.y + PIPE_HEIGHT) + sinf(v_rad) * cameraDist;

            // O ponto para onde olhamos
            float lookX = eyeX - sinf(h_rad) * cosf(v_rad);
            float lookY = eyeY + sinf(v_rad);
            float lookZ = eyeZ - cosf(h_rad) * cosf(v_rad);

            gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);
            drawSun();
        }
        break;

        case POSITIONAL_CAM: // --- MODO 2: CÂMERA EXTRA (Vista de cima) ---
        {
            // Esta câmera ignora rotação, fica fixa no topo olhando para baixo
            gluLookAt(player.x, player.y + 20.0f, player.z,
                      player.x, player.y, player.z,         
                      0.0f, 0.0f, -1.0f);   
            drawSun();                
        }
        break;

        case CAM_THIRD_PERSON: // --- MODO 0: TERCEIRA PESSOA (Padrão) ---
        default:               
        {
            // Alterado: Usamos totalAngle aqui também.
            // Assim, se o corpo do tanque rodar (A/D), a câmera roda junto.
            float camX = player.x + sinf(totalAngle * RADIAN_FACTOR) * CAM_FACTOR_X;
            float camY = CAM_FACTOR_Y - (player.pipeAngle * 0.1f);
            float camZ = player.z + cosf(totalAngle * RADIAN_FACTOR) * CAM_FACTOR_Z;

            if (camY < 1.0f) camY = 1.0f;
            if (camY > 5.0f) camY = 5.0f;

            gluLookAt(camX, camY, camZ,
                      player.x, 0.5f, player.z,
                      0.0f, 1.0f, 0.0f);
            drawSun();
        }
        break;
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
    gluLookAt(50.0f, 50.0f, 50.0f, 
              50.0f, 0.0f, 50.0f,
              0.0f, 0.0f, -1.0f); 

    drawMapSun();
}

void CalculateFreeCamNewPosition(){
    if (specialKeyStates[GLUT_KEY_LEFT]) fcAngleH -= FREE_CAM_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_RIGHT]) fcAngleH += FREE_CAM_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_UP])   fcAngleV += FREE_CAM_ROT_SPEED;
    if (specialKeyStates[GLUT_KEY_DOWN]) fcAngleV -= FREE_CAM_ROT_SPEED;

    if (fcAngleV > 89.0f) fcAngleV = 89.0f;
    if (fcAngleV < -89.0f) fcAngleV = -89.0f;

    float radH = fcAngleH * 3.14159f / 180.0f;
    float radV = fcAngleV * 3.14159f / 180.0f;

    dirX = sinf(radH) * cosf(radV);
    dirY = sinf(radV);
    dirZ = -cosf(radH) * cosf(radV);

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
    if (keyStates['q'] || keyStates['Q']) fcY -= FREE_CAM_SPEED;
    if (keyStates['e'] || keyStates['E']) fcY += FREE_CAM_SPEED;
}