#include <GL/glut.h> // Usar o header padrão do GLUT
#include <lighting.h>

static GLfloat ambient_original[] = { 0.5f, 0.5f, 0.5f, 1.0f };
static GLfloat diffuse_original[] = { 1.0f, 0.95f, 0.9f, 1.0f };

void setupLighting() {

    GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 0.95f, 0.9f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat espec[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; 
    GLint especMaterial = 10;

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Cor do céu (azul claro)

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
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void drawMapSun(){
    GLfloat light_position[] = { 0.0f, 1.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void updateFreezeLighting() {
    unsigned long now = glutGet(GLUT_ELAPSED_TIME);

    // Frozen
    GLfloat ambient_frozen[] = { 0.2f, 0.3f, 0.5f, 1.0f };
    GLfloat diffuse_frozen[] = { 0.4f, 0.6f, 1.0f, 1.0f }; 
    
    // Cor original
    GLfloat light_diffuse_original[] = { 1.0f, 0.95f, 0.9f, 1.0f }; 
    
    // Verifica se o tempo de congelamento ainda está ativo
    if (now < freezeEndTime) {
        // --- APLICAR CONGELAMENTO ---
        
        // 1. Mudar Luz Ambiente Global (Afeta a cor base de TODOS os objetos)
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_frozen);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_frozen); // A luz 0 também tem componente ambiente
        
        // 2. Mudar Luz Difusa (A luz vinda da fonte)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_frozen);
        
        // 3. Mudar a Cor de Fundo (Céu)
        glClearColor(0.2f, 0.3f, 0.7f, 1.0f); // Azul escuro
    } else {
        // --- REVERTER (Garantir que os valores voltem ao normal) ---

        // 1. Reverter Luz Ambiente Global
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_original);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_original);
        
        // 2. Reverter Luz Difusa
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_original);
        
        // 3. Reverter Cor de Fundo (Céu)
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Azul claro original
    }
}