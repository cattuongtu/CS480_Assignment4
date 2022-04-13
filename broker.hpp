#ifndef BROKER_H
#define BROKER_H


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
#include "producers.hpp"
#include "consumers.hpp"
#include "io.h"

using namespace std;

struct Broker{
    sem_t mutex, unUsedRides, availableSlots, maxHumanDrivers, maxRidesAchieved; //All Semaphores to be used
    queue<int> *ridesQueue;//Que of Requests
    bool costSaveRideBool, fastRideBool, produceRideHumanBool, produceRideRoboBool; //Booleans for optional argument inputs
    //Integers for maxamount of rides, time delays, consumed total, producerID and consumerID
    int maxRides, costSaveTime, fastRideTime, produceRideHuman, produceRideRobo, producerId, consumerId, consumed;
    //number of rides inRequestQueue on both Human and Robot
    //Number of produced rides Human and Robot
    int inRequestQueue[NUMBER_OF_TYPES], Produced[NUMBER_OF_TYPES];
    //Stores the total amount of rides consumed by
    //Cost save dispatch and fast dispatch
    int **consumedTotals;
};

#endif