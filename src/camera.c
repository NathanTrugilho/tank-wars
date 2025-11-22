#include <camera.h>

void updateCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, ratio, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    float camX = tankX + sinf(turretAngle * 3.14159 / 180.0) * CAM_FACTOR_X;  
    float camY = CAM_FACTOR_Y - (pipeAngle * 0.1f); 
    float camZ = tankZ + cosf(turretAngle * 3.14159 / 180.0) * CAM_FACTOR_Z;

    if (camY < 1.0f) camY = 1.0f; 
    if (camY > 5.0f) camY = 5.0f;

    gluLookAt(camX, camY, camZ, 
              tankX, 0.5f, tankZ,
              0.0f, 1.0f, 0.0f);
    
    drawSun();
}