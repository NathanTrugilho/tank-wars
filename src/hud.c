#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <hud.h>
#include <tank.h>

// Controle de tempo
static clock_t startTime = 0;
static double elapsedTime = 0.0;

// Função auxiliar para desenhar texto
static void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void updateHUDTime() {
    if (startTime == 0) startTime = clock();
    clock_t now = clock();
    elapsedTime = (double)(now - startTime) / CLOCKS_PER_SEC;
}

void drawHUD() {
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

    // Restaura estado original
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
