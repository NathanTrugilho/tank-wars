#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <menu.h>
#include <game.h>
#include <string.h>

extern int currentState; // declarado na main
GLuint menuBackgroundTex;

void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void drawCenteredText(float y, const char *text)
{
    int len = strlen(text);
    float width = len * 9; // ~ estimativa Helvetica18
    drawText(400 - width / 2, y, text);
}

void drawMenuFrame()
{
    glLineWidth(3);
    glColor3f(0.2f, 0.8f, 0.2f); // Verde militar HUD

    glBegin(GL_LINE_LOOP);
    glVertex2f(200, 450);
    glVertex2f(600, 450);
    glVertex2f(600, 350);
    glVertex2f(200, 350);
    glEnd();
}

void displayMenu()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Projeção 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Fundo
    glColor3f(0.05f, 0.05f, 0.05f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground(); // <<-- Fundo com blur

    // Agora desenha os textos em cima
    drawMenuFrame();
    drawCenteredText(392, "TANK BATTLE");
    drawCenteredText(260, "1. PLAY");
    drawCenteredText(120, "Pressione 1 para comecar");

    glutSwapBuffers();
}

void keyboardMenu(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1': // PLAY
        currentState = 1;

        // Restaurar projecao 3D do jogo
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);

        startGame();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutTimerFunc(16, timer, 0);
        glutKeyboardFunc(keyDown);
        glutKeyboardUpFunc(keyUp);
        break;

    case 27: // ESC
        exit(0);
        break;
    }
}

void initMenu()
{
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    menuBackgroundTex = loadBMP("tank_background.bmp");

    glutDisplayFunc(displayMenu);
    glutKeyboardFunc(keyboardMenu);
}

GLuint loadBMP(const char *filename)
{
    unsigned char header[54];
    unsigned int dataPos, width, height, imageSize;
    unsigned char *data;

    FILE *file = fopen(filename, "rb");
    if (!file)
        return 0;

    fread(header, 1, 54, file);

    dataPos = *(int *)&(header[0x0A]);
    imageSize = *(int *)&(header[0x22]);
    width = *(int *)&(header[0x12]);
    height = *(int *)&(header[0x16]);

    if (!imageSize)
        imageSize = width * height * 3;
    if (!dataPos)
        dataPos = 54;

    data = (unsigned char *)malloc(imageSize);
    fread(data, 1, imageSize, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
    return textureID;
}

void drawBackground()
{
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, menuBackgroundTex);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(800, 0);
    glTexCoord2f(1, 1);
    glVertex2f(800, 600);
    glTexCoord2f(0, 1);
    glVertex2f(0, 600);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
