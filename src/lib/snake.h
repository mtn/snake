// Interface for snake movement

typedef struct {
    int x;
    int y;
    int len; // Corresponds to score
} Snake;

void moveUp();
void moveDown();
void moveLeft();
void moveRight();

