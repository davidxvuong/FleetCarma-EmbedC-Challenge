all: main

clean:
	rm -rf bin

main:
	make clean
	mkdir bin
	gcc -o bin/main.o trip_calculator/numerical_integration.c trip_calculator/trip_calculator.c main.c