#include <game.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateCamera();
    drawSun();
    drawMap();
    drawTank();
    drawEnemies();
    debugDrawPlayerCollision();
    debugDrawEnemyCollision();

    // Desenha bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            glPushMatrix();
                glTranslatef(bullets[i].x, 0.3f, bullets[i].z);
                glColor3f(1.0f, 1.0f, 0.0f);
                glutSolidSphere(0.2, 10, 10);
            glPopMatrix();
        }
    }

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

void timer(int value) {

    updateTank();
    updateEnemies(tankX, tankZ);
    updateBullets();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); 
}

// Inicialização
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    setupLighting();
    //initBullets();
    initEnemies();
    initMapCells();
    calcularNormaisDoMapa();
    initTank();
}