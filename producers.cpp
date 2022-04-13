#include "producers.hpp"

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
     //create a new buff struct to reference incoming buff
    buffer *buff = (buffer*)arg;
    //Declares rideID
    int rideID = buff->producerId;
    ++buff->producerId;
    //Item to be inserted either human or robo request
    while(!sem_trywait(&buff->limit)){ //Checks to see if rides produced has reached MAX Rides
        if(rideID == HumanDriver){ //Checks to see if rideID is for human driver
            if(buff->produceRideHumanBool){ //Checks to see if time option was inputed for humanDriver
                usleep(buff->produceRideHuman * MULTIPLE_FOR_SECONDS); //Sleeps for given seconds
            }
            sem_wait(&buff->maxHumanDrivers); //checks to see if max human drivers is reached, if has then it waits until one is consumed
        }
        else if(rideID == RoboDriver){ //Checks to see if rideID is for robo driver
            if(buff->produceRideRoboBool){ //Checks to see if time option was inputed for roboDriver
                usleep(buff->produceRideRobo * MULTIPLE_FOR_SECONDS); //Sleeps for given seconds
            }
        }
        sem_wait(&buff->availableSlots); //Checks for available slots in queue, if no slots available then waits
        sem_wait(&buff->mutex); //Checks if it has key to access critical section
        buff->ridesQueue->push(rideID); //pushes request into queue

        ++buff->inRequestQueue[rideID]; //number of request in request Queue for given rideID
        ++buff->Produced[rideID]; //increases the number of produced riders for given rideID

        if(rideID == HumanDriver){ //Checks to see if human driver was created
            io_add_type(HumanDriver, buff->inRequestQueue, buff->Produced); //print statement for io
        }
        else if(rideID == RoboDriver){ //checks to see if robo driver was created
            io_add_type(RoboDriver, buff->inRequestQueue, buff->Produced); //print statement for io
        }
        sem_post(&buff->mutex); //releases key and exits the critical section
        sem_post(&buff->unconsumed); //increases the 
    }
    return NULL;
}