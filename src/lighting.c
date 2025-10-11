#include <lighting.h>

// Arquivo: [Teu arquivo de inicialização, e.g., game.c]

// Documentação:
// Configura todo o sistema de iluminação do OpenGL para simular um nascer do sol.
// Deve ser chamada APENAS UMA VEZ na função de inicialização (init()).
void setupLighting() {
    // ----------------------------------------------------
    // 1. HABILITAR SISTEMAS GLOBAIS
    // ----------------------------------------------------
    
    // Ligar o sistema de iluminação principal do OpenGL.
    glEnable(GL_LIGHTING);
    
    // Ligar a primeira fonte de luz (GL_LIGHT0).
    glEnable(GL_LIGHT0);
    
    // Habilitar o rastreamento de cor do material.
    // Isto permite que glColor3f() defina a cor dos objetos.
    glEnable(GL_COLOR_MATERIAL);
    
    // Associar as cores definidas com glColor3f() às propriedades
    // AMBIENTE e DIFUSA do material.
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // ----------------------------------------------------
    // 2. PROPRIEDADES DA LUZ (NASCER DO SOL)
    // ----------------------------------------------------

    // Cores: Tons quentes para simular o nascer do sol
    
    // Luz Ambiente (Geral): Uma luz suave e quente que preenche todas as áreas.
    // (Amarelo/Laranja suave)
    GLfloat light_ambient[] = { 0.4f, 0.3f, 0.1f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Luz Difusa (Direcional): A cor principal do Sol a atingir a superfície.
    // (Amarelo/Laranja claro)
    GLfloat light_diffuse[] = { 0.8f, 0.6f, 0.3f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Luz Especular (Brilho): O reflexo na superfície, geralmente branco.
    // (Branco/Amarelo)
    GLfloat light_specular[] = { 0.9f, 0.8f, 0.6f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // ----------------------------------------------------
    // 3. POSIÇÃO DA LUZ
    // ----------------------------------------------------

    // Posição da Luz: Luz Direcional (w=0.0) vinda de um ângulo de nascer do sol.
    // Documentação: 
    // - O vetor define a direção da luz.
    // - Vamos apontar para baixo (Y negativo) e ligeiramente para a frente (Z negativo).
    // - w=0.0 torna-a uma luz direcional (infinita), como o Sol.
    
    // Nota: Ajusta estes valores (X e Z) para definir o ângulo exato do "nascer".
    // { 1.0f (X), -0.5f (Y), 0.5f (Z), 0.0f (Direcional) }
}

void drawSun(){
    GLfloat light_position[] = { 1.0f, -0.5f, 0.5f, 0.0f };   
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}