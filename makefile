CC = g++ -std=c++11

rideshare: main.o producers.o consumers.o io.o
	${CC} -g -o rideshare main.o producers.o consumers.o io.o -lpthread -lrt
main.o: broker.hpp main.cpp
	${CC} -c main.cpp
producers.o: producers.hpp producers.cpp
	${CC} -c producers.cpp
consumers.o: consumers.hpp consumers.cpp
	${CC} -c consumers.cpp
io.o: io.h io.c
	${CC} -c io.c
clean:
	rm *.o rideshare