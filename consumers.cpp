#include "consumers.h"

void *Consumer(void *arg){
    //create a new buffer struct to reference incoming buffer
    buffer *new_rides = (buffer*) arg;
    Request *request;
    int consumer = new_rides->consumerId;
    ++new_rides->consumerId;
    while(new_rides->consumed != new_rides->maxRides){
        while(!sem_trywait(&new_rides->unconsumed)){
            if(consumer == 0){
                if(new_rides->costSaveRideBool){
                    usleep(new_rides->costSaveTime * MULTIPLE_FOR_SECONDS);
                }
            }
            else if(consumer == 1){
                if(new_rides->fastRideBool){
                    usleep(new_rides->fastRideTime * MULTIPLE_FOR_SECONDS);
                }
            }
            sem_wait(&new_rides->mutex); //Critical section
            request = new_rides->ridesQueue->front();
            --new_rides->inRequestQueue[request->request_id];
            new_rides->ridesQueue->pop();

            ++new_rides->totals[consumer][request->request_id]; 

            //Print statements
            if(consumer == 0){
                int costSave[RequestTypeN] = {new_rides->totals[consumer][0], new_rides->totals[consumer][1]};
                if(request->request_id == 0){
                    io_remove_type(CostAlgoDispatch, HumanDriver, new_rides->inRequestQueue, costSave);
                }
                if(request->request_id == 1){
                    io_remove_type(CostAlgoDispatch, RoboDriver, new_rides->inRequestQueue, costSave);
                }
            }
            else if(consumer == 1){
                int fastRide[RequestTypeN] = {new_rides->totals[consumer][0], new_rides->totals[consumer][1]};
                if(request->request_id == 0){
                    io_remove_type(FastAlgoDispatch, HumanDriver, new_rides->inRequestQueue, fastRide);
                }
                if(request->request_id == 1){
                    io_remove_type(FastAlgoDispatch, RoboDriver, new_rides->inRequestQueue, fastRide);
                }
            }
            if(request->request_id == 0){
                sem_post(&new_rides->maxHumanDrivers);
            }
            sem_post(&new_rides->mutex);
            sem_post(&new_rides->availableSlots);
            ++new_rides->consumed;
        } 
    }
    return NULL;
}