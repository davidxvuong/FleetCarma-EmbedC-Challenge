all: main

clean:
	rm -rf bin

main:
	mkdir bin
	gcc -o bin/main.o trip_calculator/numerical_integration.c main.c