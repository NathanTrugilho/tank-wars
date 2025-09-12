#define MAX_ENEMIES 10

// Estrutura inimigos
typedef struct {
    float x, z;
    int alive;
} Enemy;

extern Enemy enemies[MAX_ENEMIES];
