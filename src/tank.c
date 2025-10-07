#include <tank.h>
#include <projectile.h>
#include <input.h>

// Posição e orientação do tanque
float tankX = 0.0f;
float tankZ = 0.0f;
float tankAngle = 0.0f;

// Velocidade do tanque
float moveSpeed = 0.2f;
float rotSpeed = 2.0f;

const double RADIAN_FACTOR = 3.14159 / 180.0; // Pra converter de graus pra radiano

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

// Atualiza tanque
void updateTank() {
    float nextX = tankX;
    float nextZ = tankZ;

    if (keyStates['w'] || keyStates['W']) {
        nextX -= sinf(tankAngle * RADIAN_FACTOR) * moveSpeed;
        nextZ -= cosf(tankAngle * RADIAN_FACTOR) * moveSpeed;
    }
    if (keyStates['s'] || keyStates['S']) {
        nextX += sinf(tankAngle * RADIAN_FACTOR) * moveSpeed;
        nextZ += cosf(tankAngle * RADIAN_FACTOR) * moveSpeed;
    }
    if (keyStates['a'] || keyStates['A']) tankAngle += rotSpeed;
    if (keyStates['d'] || keyStates['D']) tankAngle -= rotSpeed;

    // Limites do mapa
    if (nextX > -20 && nextX < 20) tankX = nextX;
    if (nextZ > -20 && nextZ < 20) tankZ = nextZ;
}

