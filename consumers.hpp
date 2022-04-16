/*
Name: 	Raul Garcia Jr
        Cat-Tuong Tu
#RedID: 819138431
        822204923
#Class: CS480-01
#Assignment 4
*/
#ifndef CONSUMERS_H
#define CONSUMERS_H

#include "broker.hpp"
// Consumer process that removes a request from the
// bounded buffer queue and then simulates the consumption
// of a human driver request or a robot driver request.
// Once the request has been produced, it gets inserted
// into the bounded buffer queue.
// Parameters: Takes in a broker object that contains the bounded buffer,
// semaphores to avoid deadlocks, and other shared variables.
void *Consumer(void *arg);

#endif