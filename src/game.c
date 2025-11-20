#include <game.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateCamera();
    drawSun();
    drawMap();
    drawEnemies();
    drawTank();
    drawBullet();
    
    //debugDrawPlayerCollision();
    //debugDrawEnemyCollision();

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
    initMapCells();
    calcularNormaisDoMapa();
    initEnemies();
    initBullet();
    initTank();
    debugObjModel(&shellModel);
    debugObjModel(&hullModel);
    //printf("%p %p", &hullModel, &shellModel);
}