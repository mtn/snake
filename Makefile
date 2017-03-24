snake: snake.c
	gcc -lncurses snake.c -o snake

clean:
	rm -rf *.o a.out snake
