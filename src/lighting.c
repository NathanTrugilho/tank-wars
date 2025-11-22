#include <GL/glut.h> // Usar o header padrão do GLUT

void setupLighting() {

    GLfloat light_ambient[] = { 0.4f, 0.3f, 0.1f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.6f, 0.3f, 1.0f };
    GLfloat light_specular[] = { 0.9f, 0.8f, 0.6f, 1.0f };

    GLfloat espec[4]={1.0,1.0,1.0,1.0}; 
    GLint especMaterial = 60;

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT,GL_SPECULAR, espec);
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);         // Ligar o sistema de iluminação principal.
    glEnable(GL_LIGHT0);           // Ligar a primeira fonte de luz (GL_LIGHT0).
    glEnable(GL_COLOR_MATERIAL);   // Habilitar o rastreamento de cor do material.
    glEnable(GL_DEPTH_TEST);       // Habilitar teste de profundidade para renderização 3D correta.
}

void drawSun(){
    GLfloat light_position[] = { 1.0f, 0.5f, 1.0f, 0.0f }; // ultimo param diz se é posicional (1) ou direcional (0). 
                                                           // por ser direcional, ele simula uma fonte muito distante que manda raios de luz
                                                           // na direção (5, 2, 1) que são os outros parâmetros
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}