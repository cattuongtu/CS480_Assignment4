#include "producers.hpp"

void *Producer(void *arg){
     //create a new broker struct to reference
    Broker *broker = (Broker*)arg;
    //Declares first rideID
    int rideID = broker->producerId;
    ++broker->producerId;
    //Item to be inserted either human or robo request
    while(broker->produced < broker->maxRides){ //Checks to see if rides produced has reached MAX Rides
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
        
        sem_wait(&broker->availableSlots); //Checks for available slots in queue, if no slots available then waits until one becomes available
        sem_wait(&broker->mutex); //Checks if it has key to access critical section
        if(broker->produced < broker->maxRides){
            
            broker->ridesQueue->push(rideID); //pushes request into queue
            ++broker->produced; //Increases the count for produced
            ++broker->inRequestQueue[rideID]; //Increase number of request in request Queue for given rideID
            ++broker->Produced[rideID]; //increases the number of produced riders for given rideID

            if(rideID == HumanDriver){ //Checks to see if human driver was created
                io_add_type(HumanDriver, broker->inRequestQueue, broker->Produced); //print statement for io
            }
            else if(rideID == RoboDriver){ //checks to see if robo driver was created
                io_add_type(RoboDriver, broker->inRequestQueue, broker->Produced); //print statement for io
            }
        }
        sem_post(&broker->mutex); //releases key and exits the critical section
        sem_post(&broker->unUsedRides); //increases rides in queue
    }
    return NULL;
}

