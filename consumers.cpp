#include "consumers.h"

void *Consumer(void *arg){
    //create a new buffer struct to reference incoming buffer
    buffer *new_rides = (buffer*) arg;
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
}