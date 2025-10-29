#include <camera.h>

void updateCamera()
{
    float camX = playerTank.hitbox.position.x + sinf(playerTank.tankAngle * 3.14159 / 180.0) * 5.0f;  
    float camZ = playerTank.hitbox.position.z + cosf(playerTank.tankAngle * 3.14159 / 180.0) * 5.0f;
    gluLookAt(camX, 3.0f, camZ, 
              playerTank.hitbox.position.x, 0.5f, playerTank.hitbox.position.z,
              0.0f, 1.0f, 0.0f);
}