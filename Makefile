snake: src/main.c src/snake.c src/lib/snake.h src/lib/main.h
	gcc -lncurses src/main.c src/snake.c -o snake

clean:
	rm -rf *.o a.out snake
