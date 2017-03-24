snake: src/main.c src/snake.c
	gcc -lncurses src/main.c -o snake

clean:
	rm -rf *.o a.out snake
