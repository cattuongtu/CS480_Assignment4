#include "consumers.hpp"

/*void *Consumer(void *arg){
    //create a new Broker struct to reference incoming Broker
    Broker *new_rides = (Broker*) arg;
    //Request created
    Request *request;
    int consumer = new_rides->consumerId;
    ++new_rides->consumerId;
    while(new_rides->consumed != new_rides->maxRides){ //Checks to see if consumed is not equal to maxrides
        while(!sem_trywait(&new_rides->unconsumed)){ //Checks to see there are unconsumed rides in queue
            if(consumer == 0){
                if(new_rides->costSaveRideBool){ //Checks to see if time option was inputed for cost save
                    //multiplies wait time by 1000 since usleep is in microseconds
                    usleep(new_rides->costSaveTime * MULTIPLE_FOR_SECONDS);
                }
            }
            else if(consumer == 1){
                if(new_rides->fastRideBool){ //Checks to see if time option was inputed for fast drive
                    //multiplies wait time by 1000 since usleep is in microseconds
                    usleep(new_rides->fastRideTime * MULTIPLE_FOR_SECONDS);
                }
            }

            sem_wait(&new_rides->mutex); //Critical section
            request = new_rides->ridesQueue->front(); //returns the first ride request in queue
            --new_rides->inRequestQueue[request->request_id];
            new_rides->ridesQueue->pop(); //removes ride from the queue

            ++new_rides->consumedTotals[consumer][request->request_id]; //Increases the consumed totals in 2D consumedTotals array

            //Print statements
            if(consumer == 0){
                int costSave[RequestTypeN] = {new_rides->consumedTotals[consumer][0], new_rides->consumedTotals[consumer][1]};
                if(request->request_id == 0){
                    io_remove_type(CostAlgoDispatch, HumanDriver, new_rides->inRequestQueue, costSave);
                }
                if(request->request_id == 1){
                    io_remove_type(CostAlgoDispatch, RoboDriver, new_rides->inRequestQueue, costSave);
                }
            }
            else if(consumer == 1){
                int fastRide[RequestTypeN] = {new_rides->consumedTotals[consumer][0], new_rides->consumedTotals[consumer][1]};
                if(request->request_id == 0){
                    io_remove_type(FastAlgoDispatch, HumanDriver, new_rides->inRequestQueue, fastRide);
                }
                if(request->request_id == 1){
                    io_remove_type(FastAlgoDispatch, RoboDriver, new_rides->inRequestQueue, fastRide);
                }
            }
            if(request->request_id == 0){
                sem_post(&new_rides->maxHumanDrivers); //Available for another human driver due to limit of 4
            }
            sem_post(&new_rides->mutex); //End of critical section
            sem_post(&new_rides->availableSlots); //Available slot becomes open
            ++new_rides->consumed; //increases the total for consumed
        } 
    }
    return NULL;
}*/

//Test Codes
void *Consumer(void *arg){
    //create a new Broker struct to reference
    Broker *broker = (Broker*) arg;
    //Request holder
    int request;
    int consumer = broker->consumerId;
    ++broker->consumerId;
    while(broker->consumed != broker->maxRides){ //Checks to see if consumed is not equal to maxrides
        while(!sem_trywait(&broker->unconsumed)){ //Checks to see there are unconsumed rides in queue
            if(consumer == CostAlgoDispatch){
                if(broker->costSaveRideBool){ //Checks to see if time option was inputed for cost save
                    //multiplies wait time by 1000 since usleep is in microseconds
                    usleep(broker->costSaveTime * MULTIPLE_FOR_SECONDS);
                }
            }
            else if(consumer == FastAlgoDispatch){
                if(broker->fastRideBool){ //Checks to see if time option was inputed for fast drive
                    //multiplies wait time by 1000 since usleep is in microseconds
                    usleep(broker->fastRideTime * MULTIPLE_FOR_SECONDS);
                }
            }

            sem_wait(&broker->mutex); //Critical section
            request = broker->ridesQueue->front(); //returns the first ride request in queue
            --broker->inRequestQueue[request];
            broker->ridesQueue->pop(); //removes ride from the queue

            ++broker->consumedTotals[consumer][request]; //Increases the consumed totals in 2D consumedTotals array

            //Print statements
            if(consumer == CostAlgoDispatch){ //checks if consumer was cost algo
                int costSave[RequestTypeN] = {broker->consumedTotals[consumer][CostAlgoDispatch], broker->consumedTotals[consumer][FastAlgoDispatch]};
                if(request == HumanDriver){
                    sem_post(&broker->maxHumanDrivers); //If request was human, frees up space for human request from max of 4
                    io_remove_type(CostAlgoDispatch, HumanDriver, broker->inRequestQueue, costSave);
                }
                if(request == RoboDriver){
                    io_remove_type(CostAlgoDispatch, RoboDriver, broker->inRequestQueue, costSave);
                }
            }
            else if(consumer == FastAlgoDispatch){ //checks if consumer is fast algo
                int fastRide[RequestTypeN] = {broker->consumedTotals[consumer][CostAlgoDispatch], broker->consumedTotals[consumer][FastAlgoDispatch]};
                if(request == HumanDriver){ //Checks if request was a human
                    sem_post(&broker->maxHumanDrivers); //If request was human, frees up space for human request from max of 4
                    io_remove_type(FastAlgoDispatch, HumanDriver, broker->inRequestQueue, fastRide);
                }
                if(request == RoboDriver){ //checks if request was a robo
                    io_remove_type(FastAlgoDispatch, RoboDriver, broker->inRequestQueue, fastRide);
                }
            }

            sem_post(&broker->mutex); //End of critical section
            sem_post(&broker->availableSlots); //Available slot becomes open
            ++broker->consumed;
        } 
    }
    return NULL;
}