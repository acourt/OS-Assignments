CC=g++

all: Elevator Person ./main.cpp
	$(CC) main.cpp ./bin/Elevator.o ./bin/Person.o -o main.o

Elevator: ./Elevator.cpp ./Elevator.h
	$(CC) -c Elevator.cpp -o ./bin/Elevator.o
	
Person: ./Person.cpp ./Person.h
	$(CC) -c Person.cpp -o ./bin/Person.o
	
clean: 
	rm -r *.o
	rm -r ./bin/*.o
