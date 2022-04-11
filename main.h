#ifndef MAIN_H
#define MAIN_H

#define DEFAULT 0
#define MAX_RIDES 120
#define RIDE_REQUEST_MAX_SLOTS 12
#define MAX_REQUEST_HUMAN_DRIVERS 4

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

using namespace std;

class Request{
    public:
        Request(int id);
        int request_id;
};


struct buffer{
    sem_t mutex, unconsumed, availableSlots, maxHumanDrivers, limit;
    bool costSaveRideBool, fastRideBool, produceRideHumanBool, produceRideAutonomusBool;
    int productionLimit, costSaveTime, fastRideTime, produceRideHuman, produceRideAutonomus, producerId, consumerId, consumed;
    int inQueue[2], Produced[2]; //Don't understand
    int **totals;
    queue<Request*> *ridesQueue;
};

#endif