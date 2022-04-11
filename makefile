CC = g++ -std=c++11

rideshare: main.o producers.o consumers.o io.o
	${CC} -g -o rideshare main.o producers.o consumers.o io.o -lpthread -lrt
main.o: main.h main.cpp
	${CC} -c main.cpp
producers.o: main.h producers.h producers.cpp
	${CC} -c producers.cpp
consumers.o: main.h consumers.h consumers.cpp
	${CC} -c consumers.cpp
io.o: main.h io.h io.c
	${CC} -c io.c
clean:
	rm *.o rideshare