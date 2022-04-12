#include "producers.h"

void *Producer(void *arg){
    //create a new buffer struct to reference incoming buffer
    buffer * new_rides = (buffer*) arg;
    //Declares rideID
    int rideID = new_rides->producerId;
    ++new_rides->producerId;
    while(!sem_trywait(&new_rides->limit)){ //Checks to see if rides produced has reached MAX Rides
        if(rideID == 0){
            if(new_rides->produceRideHumanBool){ //Checks to see if time option was inputed for humanDriver
                //multiplies wait time by 1000 since usleep is in microseconds
                usleep(new_rides->produceRideHuman * MULTIPLE_FOR_SECONDS);
            }
            sem_wait(&new_rides->maxHumanDrivers);
        }
        else if(rideID == 1){
            if(new_rides->produceRideRoboBool){ //Checks to see if time option was inputed for roboDriver
                //multiplies wait time by 1000 since usleep is in microseconds
                usleep(new_rides->produceRideRobo * MULTIPLE_FOR_SECONDS); 
            }
        }
        Request *request = new Request(rideID); //Creates new request with the requestID
        sem_wait(&new_rides->availableSlots); //Check for avilable slots in queue
        sem_wait(&new_rides->mutex); //Enters the critical section

        new_rides->ridesQueue->push(request); //pushes request into the queue
        ++new_rides->inRequestQueue[rideID]; //number of request in request Queue
        ++new_rides->Produced[rideID]; //increases the number of produced riders

        if(rideID == 0){ //Checks to see if human driver was created
            io_add_type(HumanDriver, new_rides->inRequestQueue, new_rides->Produced);
        }
        else if(rideID == 1){ //checks to see if robo driver was created
            io_add_type(RoboDriver, new_rides->inRequestQueue, new_rides->Produced);
        }
        sem_post(&new_rides->mutex); //Exits the critical section
        sem_post(&new_rides->unconsumed);
    }
    return NULL;
}