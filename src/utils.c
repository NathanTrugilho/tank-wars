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