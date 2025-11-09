#include <GL/glut.h>
#include <stdio.h>
#include "menu.h"
#include "game.h"

extern int currentState; // declarado na main

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void displayMenu() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Projeção 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawText(350, 400, "TANK GAME");
    drawText(370, 300, "1. Play");
    drawText(370, 250, "2. Settings");
    drawText(300, 150, "Press 1 or 2 to continue...");

    glutSwapBuffers();
}

void keyboardMenu(unsigned char key, int x, int y) {
    switch (key) {
        case '1': // PLAY
            currentState = 1;

            // 🔹 Restaurar configurações 3D
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glEnable(GL_DEPTH_TEST);

            // 🔹 Inicia o jogo normalmente
            startGame();
            glutDisplayFunc(display);
            glutReshapeFunc(reshape);
            glutIdleFunc(idle);
            glutKeyboardFunc(keyDown);
            glutKeyboardUpFunc(keyUp);
            break;

        case '2': // SETTINGS
            printf("Settings ainda nao implementado!\n");
            break;

        case 27: // ESC
            exit(0);
            break;
    }
}

void initMenu() {
    glDisable(GL_DEPTH_TEST); // desativa profundidade no menu 2D
    glutDisplayFunc(displayMenu);
    glutKeyboardFunc(keyboardMenu);
}
