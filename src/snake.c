#include <stdlib.h>
#include <stdio.h>

#include "lib/snake.h"

void moveUp(){

}

void moveDown(){

}

void moveLeft(){

}

void moveRight(){

}

Snake* newSnake(){
    Snake *s = malloc(sizeof(Snake));
    if(!s){
        printf("Allocation failed");
        exit(1);
    }
    return(s);
}

