#include "producers.h"
#include "io.h"

void *Producer(void *arg){
    buffer * _rides = (buffer*) arg;
    int rideID = _rides->producerId;
    ++_rides->producerId;
    while(!sem_trywait(&_rides->limit)){
        if(rideID == 0){
            if(_rides->produceRideHumanBool){
                usleep(_rides->produceRideHuman * 1000);
            }
            sem_wait(&_rides->maxHumanDrivers);
        }
        else if(rideID == 1){
            if(_rides->produceRideAutonomusBool){
                usleep(_rides->produceRideAutonomus * 1000);
            }
        }
        Request *request = new Request(rideID);
        sem_wait(&_rides->availableSlots); //Check for avilable slots in queue
        sem_wait(&_rides->mutex); //Critical sectionn check for entrance

        _rides->ridesQueue->push(request);
        ++_rides->inQueue[rideID];
        ++_rides->Produced[rideID];

        if(rideID == 0){
            io_add_type(HumanDriver, _rides->inQueue, _rides->Produced);
        }
        else if(rideID == 1){
            io_add_type(RoboDriver, _rides->inQueue, _rides->Produced);
        }
        sem_post(&_rides->mutex);
        sem_post(&_rides->unconsumed);
    }
    return NULL;
}