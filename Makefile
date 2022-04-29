all: main.o	
	gcc  -o main main.o 

main:
	gcc -c -std=c17 -Wall -g3 -fsanitize=address main.c

clean:
	rm main.o main