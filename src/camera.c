#include <camera.h>

void updateCamera()
{
    float camX = player.position.x + sinf(player.tankAngle * 3.14159 / 180.0) * 5.0f;  
    float camZ = player.position.z + cosf(player.tankAngle * 3.14159 / 180.0) * 5.0f;
    gluLookAt(camX, 3.0f, camZ, 
        player.position.x, 0.5f, player.position.z,
              0.0f, 1.0f, 0.0f);
}