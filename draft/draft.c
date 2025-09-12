#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BULLETS 100
#define MAX_ENEMIES 10

// Posição e orientação do tanque
float tankX = 0.0f;
float tankZ = 0.0f;
float tankAngle = 0.0f;

// Velocidade do tanque
float moveSpeed = 0.2f;
float rotSpeed = 2.0f;

// Teclas
int keyStates[256] = {0};

// Estrutura bullets
typedef struct {
    float x, z;
    float angle;
    int active;
} Bullet;

// Estrutura inimigos
typedef struct {
    float x, z;
    int alive;
} Enemy;

Bullet bullets[MAX_BULLETS];
Enemy enemies[MAX_ENEMIES];

// Inicializa bullets e inimigos
void initBulletsAndEnemies() {
    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = 0;

    srand(time(NULL));
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 1;
        enemies[i].x = -18 + rand() % 36; // entre -18 e 18
        enemies[i].z = -18 + rand() % 36; // entre -18 e 18
    }
}

// Dispara bullet
void shootBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].x = tankX;
            bullets[i].z = tankZ;
            bullets[i].angle = tankAngle;
            break;
        }
    }
}

// Atualiza bullets
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x -= sinf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;
            bullets[i].z -= cosf(bullets[i].angle * 3.14159 / 180.0) * 0.4f;

            // Limites do mapa
            if (bullets[i].x < -20 || bullets[i].x > 20 || bullets[i].z < -20 || bullets[i].z > 20)
                bullets[i].active = 0;

            // Colisão com inimigos
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].alive) {
                    float dx = bullets[i].x - enemies[j].x;
                    float dz = bullets[i].z - enemies[j].z;
                    float dist = sqrtf(dx*dx + dz*dz);
                    if (dist < 0.7f) {
                        enemies[j].alive = 0;
                        bullets[i].active = 0;
                    }
                }
            }
        }
    }
}

// Teclado
void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = 1;
    if (key == ' ') shootBullet();
}
void keyUp(unsigned char key, int x, int y) { keyStates[key] = 0; }

// Atualiza tanque
void updateTank() {
    float nextX = tankX;
    float nextZ = tankZ;

    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(tankAngle * 3.14159 / 180.0) * moveSpeed;
        nextZ -= cosf(tankAngle * 3.14159 / 180.0) * moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(tankAngle * 3.14159 / 180.0) * moveSpeed;
        nextZ += cosf(tankAngle * 3.14159 / 180.0) * moveSpeed;
    }
    if (keyStates['a'] || keyStates['A']) tankAngle += rotSpeed;
    if (keyStates['d'] || keyStates['D']) tankAngle -= rotSpeed;

    // Limites do mapa
    if (nextX > -20 && nextX < 20) tankX = nextX;
    if (nextZ > -20 && nextZ < 20) tankZ = nextZ;
}

void drawTank() {
    glPushMatrix();
        glTranslatef(tankX, 0.5f, tankZ);
        glRotatef(tankAngle, 0.0f, 1.0f, 0.0f);

        glColor3f(0.7f, 0.1f, 0.1f);
        glPushMatrix();
            glScalef(1.0f, 0.5f, 2.0f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glColor3f(0.3f, 0.3f, 0.3f);
        glPushMatrix();
            glTranslatef(0.f, 0.3f, -0.8f); 
            glRotatef(-180, 1, 0, 0);
            GLUquadric* quad = gluNewQuadric();
            gluCylinder(quad, 0.1, 0.1, 1.0, 8, 2);
            gluDeleteQuadric(quad);
        glPopMatrix();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = tankX + sinf(tankAngle * 3.14159 / 180.0) * 5.0f;  
    float camZ = tankZ + cosf(tankAngle * 3.14159 / 180.0) * 5.0f;
    gluLookAt(camX, 3.0f, camZ, tankX, 0.5f, tankZ, 0.0f, 1.0f, 0.0f);


    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
        glVertex3f(-20.0f, 0.0f, -20.0f);
        glVertex3f(-20.0f, 0.0f, 20.0f);
        glVertex3f(20.0f, 0.0f, 20.0f);
        glVertex3f(20.0f, 0.0f, -20.0f);
    glEnd();

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
}

// Reshape
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ratio, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

// Programa principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Testeee");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glutMainLoop();
    return 0;
}
