#Name: 	Raul Garcia Jr
#		Cat-Tuong Tu
#RedID: 819138431
#		822204923
#Class: CS480-01
#Assignemt 3

CXX=g++

CXXFLAGS=-std=c++11 -g

rideshare : main.o producers.o consumers.o io.o
	$(CXX) $(CXXFLAGS) -o rideshare main.o producers.o consumers.o io.o -lpthread -lrt

main.o: broker.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
producers.o: producers.hpp producers.cpp
	$(CXX) $(CXXFLAGS) -c producers.cpp
consumers.o: consumers.hpp consumers.cpp
	$(CXX) $(CXXFLAGS) -c consumers.cpp
io.o: io.h io.c
	$(CXX) $(CXXFLAGS) -c io.c

clean:
	rm *.o rideshare