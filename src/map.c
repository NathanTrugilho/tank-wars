#include <map.h>

#define MAP_SIZE 100

void drawMap() {
    // Define a cor do mapa (verde, como no seu exemplo)
    glColor3f(0.2f, 0.8f, 0.2f);

    // Loop para percorrer cada linha do mapa no eixo Z
    // Começamos em -50 para centralizar o mapa na origem (0,0)
    for (int z = -MAP_SIZE / 2; z < MAP_SIZE / 2; z++) {
        
        // Para cada linha, iniciamos uma nova "tira" de triângulos.
        glBegin(GL_TRIANGLE_STRIP);

        // Loop para percorrer cada coluna do mapa no eixo X
        for (int x = -MAP_SIZE / 2; x <= MAP_SIZE / 2; x++) {
            
            // Primeiro vértice: na coluna 'x' e na linha 'z' (linha de baixo da tira)
            // O valor Y é 0.0 para manter o mapa plano.
            glVertex3f(x, 0.0f, z);
            
            // Segundo vértice: na coluna 'x' e na linha 'z + 1' (linha de cima da tira)
            glVertex3f(x, 0.0f, z + 1);
        }
        
        // Finaliza a tira de triângulos para esta linha
        glEnd();
    }
}