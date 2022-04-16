/*
Name: 	Raul Garcia Jr
        Cat-Tuong Tu
#RedID: 819138431
        822204923
#Class: CS480-01
#Assignment 4
*/
#ifndef PRODUCERS_H
#define PRODUCERS_H

#include "broker.hpp"

// Producer process that simulates the production of
// a human driver request or a robot driver request.
// Once the request has been produced, it gets inserted
// into the bounded buffer queue.
// Parameters: Takes in a broker object that contains the bounded buffer,
// semaphores to avoid deadlocks, and other shared variables.
void *Producer(void *arg);

#endif