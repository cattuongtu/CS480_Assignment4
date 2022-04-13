#include "producers.hpp"

/*void *Producer(void *arg){
    //create a new broker struct to reference incoming broker
    Broker* new_rides = (Broker*) arg;
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
     //create a new broker struct to reference
    Broker *broker = (Broker*)arg;
    //Declares rideID
    int rideID = broker->producerId;
    ++broker->producerId;
    //Item to be inserted either human or robo request
    while(!sem_trywait(&broker->limit)){ //Checks to see if rides produced has reached MAX Rides
        if(rideID == HumanDriver){ //Checks to see if rideID is for human driver
            if(broker->produceRideHumanBool){ //Checks to see if time option was inputed for humanDriver
                usleep(broker->produceRideHuman * MULTIPLE_FOR_SECONDS); //Sleeps for given seconds
            }
            sem_wait(&broker->maxHumanDrivers); //checks to see if max human drivers is reached, if has then it waits until one is consumed
        }
        else if(rideID == RoboDriver){ //Checks to see if rideID is for robo driver
            if(broker->produceRideRoboBool){ //Checks to see if time option was inputed for roboDriver
                usleep(broker->produceRideRobo * MULTIPLE_FOR_SECONDS); //Sleeps for given seconds
            }
        }
        sem_wait(&broker->availableSlots); //Checks for available slots in queue, if no slots available then waits
        sem_wait(&broker->mutex); //Checks if it has key to access critical section
        broker->ridesQueue->push(rideID); //pushes request into queue

        ++broker->inRequestQueue[rideID]; //number of request in request Queue for given rideID
        ++broker->Produced[rideID]; //increases the number of produced riders for given rideID

        if(rideID == HumanDriver){ //Checks to see if human driver was created
            io_add_type(HumanDriver, broker->inRequestQueue, broker->Produced); //print statement for io
        }
        else if(rideID == RoboDriver){ //checks to see if robo driver was created
            io_add_type(RoboDriver, broker->inRequestQueue, broker->Produced); //print statement for io
        }
        sem_post(&broker->mutex); //releases key and exits the critical section
        sem_post(&broker->unconsumed); //increases the 
    }
    return NULL;
}