#include "producers.h"

/*void *Producer(void *arg){
    //create a new buff struct to reference incoming buff
    buffer* new_rides = (buffer*) arg;
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
        sem_wait(&new_rides->availableSlots); //Check for avilable slots to prodice in queue if not waits
        
        sem_wait(&new_rides->mutex); //Enters the critical section waits for key

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
}*/


//Test Codes
void *Producer(void *arg){
    buffer *buff = (buffer*)arg;
    //Declares rideID
    int rideID = buff->producerId;
    ++buff->producerId;
    //Item to be inserted either human or robo request
    while(!sem_trywait(&buff->limit)){
        if(rideID == HumanDriver){
            if(buff->produceRideHumanBool){
                usleep(buff->produceRideHuman * MULTIPLE_FOR_SECONDS);
            }
            sem_wait(&buff->maxHumanDrivers);
        }
        else if(rideID == RoboDriver){
            if(buff->produceRideRoboBool){
                usleep(buff->produceRideRobo * MULTIPLE_FOR_SECONDS);
            }
        }
        sem_wait(&buff->availableSlots);
        sem_wait(&buff->mutex);
        buff->ridesQueue->push(rideID);

        ++buff->inRequestQueue[rideID]; //number of request in request Queue
        ++buff->Produced[rideID]; //increases the number of produced riders

        if(rideID == HumanDriver){ //Checks to see if human driver was created
            io_add_type(HumanDriver, buff->inRequestQueue, buff->Produced);
        }
        else if(rideID == RoboDriver){ //checks to see if robo driver was created
            io_add_type(RoboDriver, buff->inRequestQueue, buff->Produced);
        }
        sem_post(&buff->mutex); //Exits the critical section
        sem_post(&buff->unconsumed);
    }
    return NULL;
}