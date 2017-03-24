// Interface for snake movement

typedef struct {
    int xMin;
    int xMax;
    int yMin;
    int yMax;
} Boundaries;

typedef struct {
    int xLoc;
    int yLoc;
    Boundaries *b; // These aren't really snake properties, but very helpful
    int len; // Corresponds to score
} Snake;

void moveUp(Snake* S);
void moveDown(Snake* S);
void moveLeft(Snake* S);
void moveRight(Snake* S);
Snake* newSnake(int xMin, int xMax, int yMin, int yMax);
void delSnake(Snake* S);

