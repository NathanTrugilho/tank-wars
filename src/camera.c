#include <camera.h>

void updateCamera()
{
    float camX = tankX + sinf(tankAngle * 3.14159 / 180.0) * 5.0f;  
    float camZ = tankZ + cosf(tankAngle * 3.14159 / 180.0) * 5.0f;
    gluLookAt(camX, 3.0f, camZ, tankX, 0.5f, tankZ, 0.0f, 1.0f, 0.0f);
}