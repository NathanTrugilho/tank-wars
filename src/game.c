#include <game.h>
#include <hud.h>
#include <power_up.h>
#include <projectile.h>

float ratio;

int windowW, windowH;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ====================================
    // CENA PRINCIPAL
    // ====================================
    glViewport(0, 0, windowW, windowH);
    updateCamera();
    drawMap();
    drawEnemies();
    drawTank();
    drawBullet();
    //drawSnowFlake();
    drawExplosion();
    //drawAmmo();
    //drawHermesShoes();
    //drawFist();
    drawHealthPack();

    // HUD
    updateHUDTime();
    drawHUD();
    
    // ====================================
    // MINIMAPA 
    // ====================================
    int mapSize = windowW / 7; // Mudar o valor da divisão pra alterar o tamanho do mapa
    // Posição: Canto direito (W - mapSize), Canto superior (H - mapSize)
    int mapX = windowW - mapSize;
    int mapY = windowH - mapSize; 

    // DESENHAR A BORDA PRETA
    int borda = 4; // Espessura da borda em pixels
    glEnable(GL_SCISSOR_TEST);
    // Define a área de recorte
    glScissor(mapX - borda, mapY - borda, mapSize + (borda * 2), mapSize + (borda * 2));
    // Define a cor de limpeza para PRETO
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Pinta apenas o retângulo definido pelo glScissor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // RESTAURA A COR DO CÉU
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

    // DESENHAR O CONTEÚDO DO MAPA 
    glViewport(mapX, mapY, mapSize, mapSize);
    glClear(GL_DEPTH_BUFFER_BIT);
    updateMinimapCamera(); 
    glDisable(GL_LIGHTING); // Para deixar o mapa com cor plana
    drawMap();
    drawEnemies();
    drawTank();
    drawBullet();
    glEnable(GL_LIGHTING); 

    glutSwapBuffers();
}

// Não mudar
void reshape(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    ratio = (GLfloat)w/(GLfloat)h;
    glViewport(0, 0, w, h);
    updateCamera();

    windowW = w;
    windowH = h;
}

void timer(int value) {
    updateTank();
    updateEnemies(player.x, player.z);
    updateBullets();
    //testePowerUp();
    updateExplosion();

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
    initBullet();
    initPowerUps();
}