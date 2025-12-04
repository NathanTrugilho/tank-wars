#include <camera.h>
#include <input.h> // Precisamos ler keyStates aqui para mover a câmera
#include <stdio.h>

// Inicializa variáveis globais
int freeCameraMode = FALSE;
int currentCameraMode = CAM_THIRD_PERSON;

float fcAngleH;
float fcAngleV;
float fcX, fcY, fcZ;

float dirX;
float dirY;
float dirZ;

void updateCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 200); // Aumentei um pouco o alcance de visão para 200

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

    // Lógica dos 3 Modos de Câmera de Jogo
    switch (currentCameraMode) {

        case CAM_FIRST_PERSON: // --- MODO 1: PRIMEIRA PESSOA ---
        {
            float h_rad = player.turretAngle * RADIAN_FACTOR;
            float v_rad = player.pipeAngle * RADIAN_FACTOR;
            float horizontal_dist = cosf(v_rad) * PIPE_LENGTH;
            float right_x = cosf(h_rad);
            float right_z = -sinf(h_rad);

            // Olho na ponta do cano
            float eyeX = player.x - sinf(h_rad) * horizontal_dist - (right_x * BULLET_SIDE_CORRECTION);
            float eyeZ = player.z - cosf(h_rad) * horizontal_dist - (right_z * BULLET_SIDE_CORRECTION);
            float eyeY = (player.y + PIPE_HEIGHT) + sinf(v_rad) * PIPE_LENGTH;

            // Olhando para frente
            float lookX = eyeX - sinf(h_rad) * cosf(v_rad);
            float lookY = eyeY + sinf(v_rad);
            float lookZ = eyeZ - cosf(h_rad) * cosf(v_rad);

            gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);
            drawSun();
        }
        break;

        case POSITIONAL_CAM: // --- MODO 2: CÂMERA EXTRA (Vista de cima) ---
        {
            gluLookAt(player.x, player.y + 30.0f, player.z,
                      player.x, player.y, player.z,         
                      0.0f, 0.0f, -1.0f);                   
        }
        break;

        case CAM_THIRD_PERSON: // --- MODO 0: TERCEIRA PESSOA (Padrão) ---
        default:               // Se der ruim, vem pra cá
        {
            float camX = player.x + sinf(player.turretAngle * RADIAN_FACTOR) * CAM_FACTOR_X;
            float camY = CAM_FACTOR_Y - (player.pipeAngle * 0.1f);
            float camZ = player.z + cosf(player.turretAngle * RADIAN_FACTOR) * CAM_FACTOR_Z;

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

        // Conversão para Radianos
        float radH = fcAngleH * 3.14159f / 180.0f;
        float radV = fcAngleV * 3.14159f / 180.0f;

        // Vetor "Frente" (para onde estou olhando)
        // No OpenGL o Z é 'para fora', então invertemos algumas coisas dependendo da lógica
        dirX = sinf(radH) * cosf(radV);
        dirY = sinf(radV);
        dirZ = -cosf(radH) * cosf(radV); // Z negativo é "para frente" no padrão gluLookAt

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