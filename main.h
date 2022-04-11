#ifndef MAIN_H
#define MAIN_H


//Defines Values for all potential magic numbers in program
//Name is description
#define DEFAULT 0
#define MAX_RIDES_DEFAULT 120
#define RIDE_REQUEST_MAX_SLOTS 12
#define MAX_REQUEST_HUMAN_DRIVERS 4
#define MULTIPLE_FOR_SECONDS 1000
#define NUMBER_OF_TYPES 2

//All includes to be used
#include <queue>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <semaphore.h>
#include <string>
#include <pthread.h>
#include "ridesharing.h"
#include "producers.h"
#include "consumers.h"
#include "io.h"

using namespace std;

//Class decleration for Request that holds request_id
class Request{
    public:
        Request(int id);
        int request_id;
};


//Buffer Structure that holds all relavent variables, queues, and arrays to be used in all the files
struct buffer{
    sem_t mutex, unconsumed, availableSlots, maxHumanDrivers, limit; //All Semaphores to be used
    bool costSaveRideBool, fastRideBool, produceRideHumanBool, produceRideAutonomusBool;
    int maxRides, costSaveTime, fastRideTime, produceRideHuman, produceRideAutonomus, producerId, consumerId, consumed;
    int inRequestQueue[2], Produced[2]; //Don't understand
    int **totals;
    queue<Request*> *ridesQueue;
};

#endif