#include <GL/glut.h> // Usar o header padrão do GLUT

void setupLighting() {
    // ----------------------------------------------------
    // 1. HABILITAR SISTEMAS GLOBAIS
    // ----------------------------------------------------
    glEnable(GL_LIGHTING);         // Ligar o sistema de iluminação principal.
    glEnable(GL_LIGHT0);           // Ligar a primeira fonte de luz (GL_LIGHT0).
    glEnable(GL_COLOR_MATERIAL);   // Habilitar o rastreamento de cor do material.
    glEnable(GL_DEPTH_TEST);       // Habilitar teste de profundidade para renderização 3D correta.

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Luz Ambiente (Geral): Uma luz suave e quente que preenche todas as áreas.
    GLfloat light_ambient[] = { 0.4f, 0.3f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Luz Difusa (Direcional): A cor principal do Sol a atingir a superfície.
    GLfloat light_diffuse[] = { 0.8f, 0.6f, 0.3f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Luz Especular (Brilho): O reflexo na superfície.
    GLfloat light_specular[] = { 0.9f, 0.8f, 0.6f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void drawSun(){
    GLfloat light_position[] = { 1.0f, 0.5f, 1.0f, 0.0f }; // ultimo param diz se é posicional (1) ou direcional (0). 
                                                           // por ser direcional, ele simula uma fonte muito distante que manda raios de luz
                                                           // na direção (5, 2, 1) que são os outros parâmetros

    //GLfloat light_position[] = { 0.0f, 1.0f, 0.0f, 0.0f }; // Sol de meio dia pra testar
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void setupDebugLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat global_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat light_position[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}