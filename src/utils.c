#include <utils.h>

vertex calcularNormalTriangulo(vertex p1, vertex p2, vertex p3) {
    vertex v, w, normal;

    // Faço os vetores
    v.x = p2.x - p1.x; 
    v.y = p2.y - p1.y; 
    v.z = p2.z - p1.z;

    w.x = p3.x - p1.x;
    w.y = p3.y - p1.y; 
    w.z = p3.z - p1.z;

    // Calculo a normal (Determinante matriz 3x3)
    normal.x = (v.y * w.z) - (v.z * w.y);
    normal.y = (v.z * w.x) - (v.x * w.z);
    normal.z = (v.x * w.y) - (v.y * w.x);

    float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    if (magnitude > 0.0f) {
        normal.x /= magnitude; 
        normal.y /= magnitude;
        normal.z /= magnitude;
    }
    return normal;
}

void calcularNormaisDoMapa() {
    /// Inicializo a matriz com zeros
    for (int z = 0; z <= MAP_SIZE; z++) {
        for (int x = 0; x <= MAP_SIZE; x++) {
            vertexNormals[z][x].x = 0.0f;
            vertexNormals[z][x].y = 0.0f;
            vertexNormals[z][x].z = 0.0f;
        }
    }

    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            
            vertex pA = mapCells[z][x].A;
            vertex pB = mapCells[z][x].B;
            vertex pC = mapCells[z][x].C;
            vertex pD = mapCells[z][x].D;

            // Divido o Quad em dois triângulos
            vertex normal1 = calcularNormalTriangulo(pA, pB, pC);
            vertex normal2 = calcularNormalTriangulo(pB, pC, pD);

            // Adiciona as normais aos vértices correspondentes na grade
            vertexNormals[z][x].x += normal1.x; // Vértice A é afetado pelo triângulo 1
            vertexNormals[z][x].y += normal1.y;
            vertexNormals[z][x].z += normal1.z;

            vertexNormals[z+1][x].x += normal1.x + normal2.x; // Vértice B é afetado por ambos
            vertexNormals[z+1][x].y += normal1.y + normal2.y;
            vertexNormals[z+1][x].z += normal1.z + normal2.z;

            vertexNormals[z][x+1].x += normal1.x + normal2.x; // Vértice C é afetado por ambos
            vertexNormals[z][x+1].y += normal1.y + normal2.y;
            vertexNormals[z][x+1].z += normal1.z + normal2.z;

            vertexNormals[z+1][x+1].x += normal2.x; // Vértice D é afetado pelo triângulo 2
            vertexNormals[z+1][x+1].y += normal2.y;
            vertexNormals[z+1][x+1].z += normal2.z;
        }
    }

    // Passo 6: Normalizar todos os vetores da grade
    for (int z = 0; z <= MAP_SIZE; z++) {
        for (int x = 0; x <= MAP_SIZE; x++) {
            vertex* n = &vertexNormals[z][x];
            float mag = sqrt(n->x * n->x + n->y * n->y + n->z * n->z);
            if (mag > 0.0f) { n->x /= mag; n->y /= mag; n->z /= mag; }
        }
    }
}