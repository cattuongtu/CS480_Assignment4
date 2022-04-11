#include "producers.h"

void *Producer(void *arg){
    //create a new buffer struct to reference incoming buffer
    buffer * new_rides = (buffer*) arg;
    int rideID = new_rides->producerId;
    ++new_rides->producerId;
    while(!sem_trywait(&new_rides->limit)){
        if(rideID == 0){
            if(new_rides->produceRideHumanBool){
                //multiplies wait time by 1000 sinc usleep is in microseconds
                usleep(new_rides->produceRideHuman * MULTIPLE_FOR_SECONDS);
            }
            sem_wait(&new_rides->maxHumanDrivers);
        }
        else if(rideID == 1){
            if(new_rides->produceRideRoboBool){
                //multiplies wait time by 1000 sinc usleep is in microseconds
                usleep(new_rides->produceRideRobo * MULTIPLE_FOR_SECONDS); 
            }
        }
        Request *request = new Request(rideID);
        sem_wait(&new_rides->availableSlots); //Check for avilable slots in queue
        sem_wait(&new_rides->mutex); //Critical sectionn check for entrance

        new_rides->ridesQueue->push(request);
        ++new_rides->inRequestQueue[rideID];
        ++new_rides->Produced[rideID];

        if(rideID == 0){
            io_add_type(HumanDriver, new_rides->inRequestQueue, new_rides->Produced);
        }
        else if(rideID == 1){
            io_add_type(RoboDriver, new_rides->inRequestQueue, new_rides->Produced);
        }
        sem_post(&new_rides->mutex);
        sem_post(&new_rides->unconsumed);
    }
    return NULL;
}