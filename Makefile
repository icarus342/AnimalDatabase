animalbin : hw5.o animals.o
	gcc -o animalbin hw5.o animals.o
hw5.o : hw5.c
	gcc -c hw5.c
animals.o : animals.c
	gcc -c animals.c
