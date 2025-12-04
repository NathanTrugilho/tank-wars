#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <hud.h>
#include <tank.h>
#include <math.h>
#include <camera.h> // <--- NECESSÁRIO para acessar currentCameraMode e CAM_FIRST_PERSON

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

// --- NOVA FUNÇÃO: Desenha a Mira (Crosshair) ---
void drawCrosshair() {
    // Só desenha se estiver em Primeira Pessoa
    if (currentCameraMode != CAM_FIRST_PERSON) return;

    float centerX = 400.0f; // Metade de 800
    float centerY = 300.0f; // Metade de 600
    float size = 15.0f;     // Tamanho da linha da mira

    glDisable(GL_TEXTURE_2D); // Garante que não aplica textura nas linhas
    glLineWidth(2.0f);        // Deixa a linha um pouco mais grossa

    glColor3f(0.0f, 1.0f, 0.0f); // Cor Verde brilhante

    glBegin(GL_LINES);
        // Linha Horizontal
        glVertex2f(centerX - size, centerY);
        glVertex2f(centerX + size, centerY);

        // Linha Vertical
        glVertex2f(centerX, centerY - size);
        glVertex2f(centerX, centerY + size);
    glEnd();

    glLineWidth(1.0f); // Retorna a espessura original para não afetar outras coisas
    glColor3f(1.0f, 1.0f, 1.0f); // Retorna para branco
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
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    int minutes = (int)(elapsedTime / 60);
    int seconds = (int)elapsedTime % 60;
    int millis = (int)((elapsedTime - (int)elapsedTime) * 1000);
    char timeText[32];
    snprintf(timeText, sizeof(timeText), "Tempo: %02d:%02d:%02d", minutes, seconds, millis / 10);

    char vidaText[32];
    snprintf(vidaText, sizeof(vidaText), "Vida: %d", player.health);

    char ammoText[32];
    snprintf(ammoText, sizeof(ammoText), "Ammo: %d", player.ammo);

    char cameraText[32];
    snprintf(cameraText, sizeof(cameraText), "Camera: %d", currentCameraMode);

    glColor3f(1, 1, 1); // Branco
    drawText(10, 570, vidaText);
    drawText(10, 545, ammoText);
    drawText(10, 520, timeText);
    drawText(10, 495, cameraText);

    drawCrosshair();

    drawReloadCircle(player.flagReloadCircle);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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
    // Nota: Como o ortho é 0..800, 0..600, vamos converter essas coordenadas normalizadas
    // A função original usava outra projeção local, vamos respeitar a lógica original dela
    // pois ela cria a própria projeção internamente.
    
    // ... [O código original de drawReloadCircle mantém-se inalterado] ...
    // Vou reincluir apenas o início para contexto, mas podes manter a função igualzinha
    
    float circleX = 0.5;  
    float circleY = 0.6f;    
    float radius = 0.05f;  
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1); 
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glColor3f(0.3f, 0.3f, 0.3f); 
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(circleX, circleY); 
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * RADIAN_FACTOR;
        glVertex2f(circleX + cosf(angle) * radius, 
                   circleY + sinf(angle) * radius);
    }
    glEnd();
    
    if (progress > 0.0f) {
        float r = 1.0f - progress;
        float g = progress;
        float b = 0.0f;
        glColor3f(r, g, b);
        
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(circleX, circleY); 
        
        int segments = (int)(360 * progress);
        for (int i = 0; i <= segments; i += 5) {
            float angle = i * RADIAN_FACTOR;
            glVertex2f(circleX + cosf(angle) * radius, 
                       circleY + sinf(angle) * radius);
        }
        
        if (segments < 360) {
            float finalAngle = segments * RADIAN_FACTOR;
            glVertex2f(circleX + cosf(finalAngle) * radius, 
                       circleY + sinf(finalAngle) * radius);
        }
        glEnd();
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}