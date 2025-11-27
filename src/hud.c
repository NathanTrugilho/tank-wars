#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <hud.h>
#include <tank.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

// Controle de tempo
static clock_t startTime = 0;
static double elapsedTime = 0.0;

// Função auxiliar para desenhar texto
static void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void updateHUDTime()
{
    if (startTime == 0)
        startTime = clock();
    clock_t now = clock();
    elapsedTime = (double)(now - startTime) / CLOCKS_PER_SEC;
}

void drawHUD()
{
    // Salva o estado atual
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Modo 2D fixo
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // HUD deve ficar por cima do 3D

    // 🔹 Tempo decorrido formatado
    int minutes = (int)(elapsedTime / 60);
    int seconds = (int)elapsedTime % 60;
    int millis = (int)((elapsedTime - (int)elapsedTime) * 1000);
    char timeText[32];
    snprintf(timeText, sizeof(timeText), "Tempo: %02d:%02d:%02d", minutes, seconds, millis / 10);

    // 🔹 Informações de HUD
    char vidaText[32];
    snprintf(vidaText, sizeof(vidaText), "Vida: %d", player.health);

    char ammoText[32];
    snprintf(ammoText, sizeof(ammoText), "Ammo: %d", player.ammo);

    // 🔹 Desenha textos
    glColor3f(1, 1, 1); // Branco
    drawText(10, 570, vidaText);
    drawText(10, 545, ammoText);
    drawText(10, 520, timeText);

    drawReloadCircle(player.flagReloadCircle);

    // Restaura estado original
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Desenha o círculo de recarga
void drawReloadCircle(int shot) {
    if (shot == FALSE) return;

    unsigned long now = glutGet(GLUT_ELAPSED_TIME);
    unsigned long timeSinceLastShot = now - player.lastShootTime;
    
    // Se não estiver em recarga, não desenha nada
    if (timeSinceLastShot >= player.reloadTime) return;
    
    // Calcula o progresso da recarga (0.0 a 1.0)
    float progress = (float)timeSinceLastShot / (float)player.reloadTime;
    
    // Posição fixa na tela (canto superior direito)
    float circleX = 0.5;  // Coordenada X normalizada
    float circleY = 0.6f;  // Coordenada Y normalizada  
    float radius = 0.05f;  // Raio do círculo
    
    // Salva a matriz de projeção atual
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1); // Coordenadas normalizadas
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Desabilita recursos desnecessários para HUD
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Desenha o círculo de fundo (contorno)
    glColor3f(0.3f, 0.3f, 0.3f); // Cinza escuro
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(circleX, circleY); // Centro
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * RADIAN_FACTOR;
        glVertex2f(circleX + cosf(angle) * radius, 
                   circleY + sinf(angle) * radius);
    }
    glEnd();
    
    // Desenha a parte preenchida progressivamente
    if (progress > 0.0f) {
        // Cor muda do vermelho para o verde conforme progride
        float r = 1.0f - progress;
        float g = progress;
        float b = 0.0f;
        glColor3f(r, g, b);
        
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(circleX, circleY); // Centro
        
        // Desenha o arco progressivo
        int segments = (int)(360 * progress);
        for (int i = 0; i <= segments; i += 5) {
            float angle = i * RADIAN_FACTOR;
            glVertex2f(circleX + cosf(angle) * radius, 
                       circleY + sinf(angle) * radius);
        }
        
        // Se não estiver completo, fecha o arco com o centro
        if (segments < 360) {
            float finalAngle = segments * RADIAN_FACTOR;
            glVertex2f(circleX + cosf(finalAngle) * radius, 
                       circleY + sinf(finalAngle) * radius);
        }
        glEnd();
    }
    
    // Restaura as configurações
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}