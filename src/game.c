#include <game.h>
#include <hud.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateCamera();
    drawSun();
    
    // Desenha mapa
    drawMap();
    
    // Desenha tanque
    drawTank();

    // Desenha inimigos
    drawEnemies();

    // Desenha bullets
    drawBullets();

    // HUD
    updateHUDTime();
    drawHUD();

    glutSwapBuffers();
}

// Não mudar
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / (float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

// Animação
void idle() {
    updateTank();
    updateBullets();
    glutPostRedisplay();
}

// Inicialização
void startGame() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    setupLighting();
    initBullets();
    initEnemies();
    initMapCells();
    calcularNormaisDoMapa();
    initTank();
}