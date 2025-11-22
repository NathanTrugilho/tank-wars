#include <game.h>

float ratio;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();

    drawMap();
    drawEnemies();
    drawTank();
    drawBullet();
    
    // Desenha balas inimigas
    drawEnemyBullets();

    debugDrawPlayerCollision();
    debugDrawEnemyCollision();

    glutSwapBuffers();
}

// Não mudar
void reshape(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    ratio = (GLfloat)w/(GLfloat)h;
    glViewport(0, 0, w, h);
    updateCamera();
}

void timer(int value) {
    updateTank();
    updateEnemies(tankX, tankZ);
    updateBullets();
    
    // Atualiza balas inimigas
    updateEnemyBullets();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); 
}

// Inicialização
void init() {
    glEnable(GL_NORMALIZE); 
    setupLighting();
    initMapCells();
    calcularNormaisDoMapa();
    initEnemies();
    initTank();
    initBullet(); // Isso já chama initEnemyBullets dentro do projectile.c atualizado
}