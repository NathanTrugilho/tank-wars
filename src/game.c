#include <game.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    updateCamera();
    
    // Desenha mapa
    drawMap();
    // Desenha tanque
    drawTank();

    // Desenha inimigos
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive) {
            glPushMatrix();
                glTranslatef(enemies[i].x, 0.5f, enemies[i].z);
                glColor3f(0.1f, 0.1f, 0.7f);
                glScalef(1.0f, 0.5f, 2.0f);
                glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

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

// Animação
void idle() {
    updateTank();
    updateBullets();
    glutPostRedisplay();
}

// Inicialização
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    initBulletsAndEnemies();
    initMapCells();
}