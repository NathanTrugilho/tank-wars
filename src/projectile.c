#include <projectile.h>

Bullet player_bullet;
ObjModel shellModel;

Explosion explosion;

// Atualiza bullets
void updateBullets()
{
    if (!player_bullet.active)
        return;

    float horizontal_without_vertical = player_bullet.horizontal_angle * RADIAN_FACTOR;

    float horizontal_scale = cosf(player_bullet.vertical_angle * RADIAN_FACTOR);

    float vector_x = -sinf(horizontal_without_vertical) * horizontal_scale;
    float vector_z = -cosf(horizontal_without_vertical) * horizontal_scale;
    float vector_y = sinf(player_bullet.vertical_angle * RADIAN_FACTOR);

    player_bullet.x += vector_x * BULLET_SPEED;
    player_bullet.z += vector_z * BULLET_SPEED;
    player_bullet.y += vector_y * BULLET_SPEED;

    // Fazer a colisão da bullet com os inimigos usando a lógica do Pedro
    for (int j = 0; j < MAX_ENEMIES; j++)
    {
        if (!enemies[j].alive)
            continue;

        float dx = player_bullet.x - enemies[j].x;
        float dz = player_bullet.z - enemies[j].z;
        float dist = sqrtf(dx * dx + dz * dz);

        if (dist < 0.7f)
        {
            startExplosion(player_bullet.x, player_bullet.y, player_bullet.z);
            enemies[j].alive = FALSE;
            player_bullet.active = FALSE;
            break;
        }
    }
}

// Carrega o modelo da bullet
void initBullet()
{
    if (loadOBJ("objects/shell.obj", "objects/shell.mtl", &shellModel))
    {
    }
    else
    {
        printf("ERRO: Nao foi possivel carregar o modelo do projetil.\n");
    }
    player_bullet.active = FALSE;
}

// Dispara bullet
void shootBullet()
{
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);

    player.flagReloadCircle = 1; // TRUE

    // Serve pra botar um cooldown antes de atirar dnv
    if (now - player.lastShootTime < player.reloadTime)
        return;
    player.lastShootTime = now;

    player_bullet.active = TRUE;

    player_bullet.horizontal_angle = player.turretAngle; // inclinação horizontal
    player_bullet.vertical_angle = player.pipeAngle;     // inclinação vertical

    float h_rad = player.turretAngle * RADIAN_FACTOR;
    float v_rad = player.pipeAngle * RADIAN_FACTOR;

    float horizontal_dist = cosf(v_rad) * PIPE_LENGTH;

    float right_x = cosf(h_rad);
    float right_z = -sinf(h_rad);

    player_bullet.x = player.x - sinf(h_rad) * horizontal_dist - (right_x * BULLET_SIDE_CORRECTION);
    player_bullet.z = player.z - cosf(h_rad) * horizontal_dist - (right_z * BULLET_SIDE_CORRECTION);
    player_bullet.y = (player.y + PIPE_HEIGHT) + sinf(v_rad) * PIPE_LENGTH;
}

void drawBullet()
{
    if (!player_bullet.active)
        return;

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(player_bullet.x, player_bullet.y, player_bullet.z);
    glScalef(BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION, BULLET_SCALE_CORRECTION);
    glRotatef(player_bullet.horizontal_angle, 0.0f, 1.0f, 0.0f);
    glRotatef(player_bullet.vertical_angle, 1.0f, 0.0f, 0.0f);

    drawModel(&shellModel);
    // drawBox(hullModel.box); // Hitbox da base

    glPopMatrix();
    glDisable(GL_TEXTURE_2D); // Tem que desativar se não fica tudo escuro pq as outras coisas n têm textura
}

void startExplosion(float x, float y, float z) {
    explosion.x = x;
    explosion.y = y + 1;
    explosion.z = z;

    explosion.radius = 0.0f;
    explosion.maxRadius = 1.5f;
    explosion.active  = 1;

    explosion.startTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 
}

// Valores de tempo da animação da explosão
const float EXPLOSION_GROW_TIME = 0.5f;   // tempo para crescer ao tamanho máximo
const float EXPLOSION_FADE_TIME = 0.5f;   // tempo para desaparecer
const float EXPLOSION_TOTAL_TIME = 1.0f;  // tempo total = crescer (2s) + sumir (1s)

void updateExplosion() {
    if (!explosion.active) return;

    // Tempo atual em segundos
    float now = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // Quanto tempo passou desde o início da explosão
    float t = now - explosion.startTime;

    // -------------------------------
    // FASE 1 — Crescimento (0 → 2s)
    // -------------------------------
    if (t <= EXPLOSION_GROW_TIME) {

        // k varia de 0 a 1 proporcionalmente em 2 segundos
        float k = t / EXPLOSION_GROW_TIME;

        // Raio interpolado entre 0 e maxRadius
        explosion.radius = explosion.maxRadius * k;
        return;
    }

    // -------------------------------
    // FASE 2 — Desaparecimento (2 → 3s)
    // -------------------------------
    if (t <= EXPLOSION_GROW_TIME + EXPLOSION_FADE_TIME) {

        // Tempo relativo da fase de fade (0 → 1)
        float fadeElapsed = t - EXPLOSION_GROW_TIME;

        // k varia de 0 a 1 ao longo de 1 segundo
        float k = fadeElapsed / EXPLOSION_FADE_TIME;

        // Raio diminui de maxRadius até 0
        explosion.radius = explosion.maxRadius * (1.0f - k);
        return;
    }

    // -------------------------------
    // FIM — explosão chegou ao fim
    // -------------------------------
    explosion.radius = 0.0f;
    explosion.active = 0;
}


void drawExplosion() {
    if (!explosion.active) return;

    float now = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float t = now - explosion.startTime;

    // Alpha inicial = 100%
    float alpha = 1.0f;

    // Após o tempo de crescimento (2s), começa o fade out
    if (t > EXPLOSION_GROW_TIME) {

        // tempo passado dentro da fase de fade
        float fadeElapsed = t - EXPLOSION_GROW_TIME;

        // alpha decai de 1 → 0 em EXPLOSION_FADE_TIME segundos
        alpha = 1.0f - (fadeElapsed / EXPLOSION_FADE_TIME);

        if (alpha < 0.0f) alpha = 0.0f;
    }

    glPushMatrix();
    glTranslatef(explosion.x, explosion.y, explosion.z);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Laranja com transparência controlada
    glColor4f(1.0f, 0.5f, 0.0f, alpha);

    // A esfera representa a explosão
    glutSolidSphere(explosion.radius, 20, 20);

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}