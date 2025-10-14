#include <GL/glut.h>
#include <projectile.h>
#include <input.h>

// Posição e orientação do tanque
extern float tankX;
extern float tankZ;
extern float tankAngle;
extern float moveSpeed;
extern float rotSpeed;

void drawTank();
// Atualiza tanque
void updateTank();