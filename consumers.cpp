#include "consumers.h"
#include "io.h"

void *Consumer(void *arg){
    buffer *_rides = (buffer*) arg;
    Request *request;
    int consumer = _rides->consumerId;
    ++_rides->consumerId;
    while(_rides->consumed != MAX_RIDES){
        while(!sem_trywait(&_rides->unconsumed)){
            if(consumer == 0){
                if(_rides->costSaveRideBool){
                    usleep(_rides->costSaveTime * 1000);
                }
            }
            else if(consumer == 1){
                if(_rides->fastRideBool){
                    usleep(_rides->fastRideTime * 1000);
                }
            }
            sem_wait(&_rides->mutex); //Critical section
            request = _rides->ridesQueue->front();
            --_rides->inQueue[request->request_id];
            _rides->ridesQueue->pop();

            ++_rides->totals[consumer][request->request_id]; 

            //Print statements
            if(consumer == 0){
                int costSave[RequestTypeN] = {_rides->totals[consumer][0], _rides->totals[consumer][1]};
                if(request->request_id == 0){
                    io_remove_type(CostAlgoDispatch, HumanDriver, _rides->inQueue, costSave);
                }
                if(request->request_id == 1){
                    io_remove_type(CostAlgoDispatch, RoboDriver, _rides->inQueue, costSave);
                }
            }
            else if(consumer == 1){
                int fastRide[RequestTypeN] = {_rides->totals[consumer][0], _rides->totals[consumer][1]};
                if(request->request_id == 0){
                    io_remove_type(FastAlgoDispatch, HumanDriver, _rides->inQueue, fastRide);
                }
                if(request->request_id == 1){
                    io_remove_type(FastAlgoDispatch, RoboDriver, _rides->inQueue, fastRide);
                }
            }
            if(request->request_id == 0){
                sem_post(&_rides->maxHumanDrivers);
            }
            sem_post(&_rides->mutex);
            sem_post(&_rides->availableSlots);
            ++_rides->consumed;
        } 
    }
    return NULL;
}