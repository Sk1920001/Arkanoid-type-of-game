all: salida

main.o: main.c
	gcc -c main.c

fuctions.o: functions.c 
	gcc -c fuctions.c

salida: main.o functions.o
	gcc -o game functions.o main.o -lm -lSDL2 -lSDL2_ttf

clean:
	rm game functions.o main.o
