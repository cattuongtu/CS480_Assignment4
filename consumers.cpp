#include "consumers.hpp"

void *Consumer(void *arg)
{
    // create a new Broker struct to reference
    Broker *broker = (Broker *)arg;
    // Request holder
    int requestID;
    int requester = broker->consumerId;
    ++broker->consumerId;
    while (broker->consumed < broker->maxRides)
    {                                   // Checks to see if consumed is not equal to maxrides
        sem_wait(&broker->unUsedRides); // Checks for unused
        sem_wait(&broker->mutex);       // Critical section
        if (broker->consumed < broker->maxRides)
        {

            requestID = broker->ridesQueue->front(); // returns the first ride requestID in queue
            --broker->inRequestQueue[requestID];
            broker->ridesQueue->pop(); // removes ride from the queue
            ++broker->consumed;
            ++broker->consumedTotals[requester][requestID]; // Increases the consumed totals in 2D consumedTotals array

            // Print statements
            if (requester == CostAlgoDispatch)
            { // checks if requester was cost algo
                int costSave[RequestTypeN] = {broker->consumedTotals[requester][CostAlgoDispatch], broker->consumedTotals[requester][FastAlgoDispatch]};
                if (requestID == HumanDriver)
                {
                    sem_post(&broker->maxHumanDrivers); // If requestID was human, frees up space for human requestID from max of 4
                    io_remove_type(CostAlgoDispatch, HumanDriver, broker->inRequestQueue, costSave);
                }
                if (requestID == RoboDriver)
                {
                    io_remove_type(CostAlgoDispatch, RoboDriver, broker->inRequestQueue, costSave);
                }
            }
            else if (requester == FastAlgoDispatch)
            { // checks if requester is fast algo
                int fastRide[RequestTypeN] = {broker->consumedTotals[requester][CostAlgoDispatch], broker->consumedTotals[requester][FastAlgoDispatch]};
                if (requestID == HumanDriver)
                {                                       // Checks if requestID was a human
                    sem_post(&broker->maxHumanDrivers); // If requestID was human, frees up space for human requestID from max of 4
                    io_remove_type(FastAlgoDispatch, HumanDriver, broker->inRequestQueue, fastRide);
                }
                if (requestID == RoboDriver)
                { // checks if requestID was a robo
                    io_remove_type(FastAlgoDispatch, RoboDriver, broker->inRequestQueue, fastRide);
                }
            }
        }
        sem_post(&broker->mutex);          // End of critical section
        sem_post(&broker->availableSlots); // Available slot becomes open
        if (requester == CostAlgoDispatch)
        {
            if (broker->costSaveRideBool)
            { // Checks to see if time option was inputed for cost save
                // multiplies wait time by 1000 since usleep is in microseconds
                usleep(broker->costSaveTime * MULTIPLE_FOR_SECONDS);
            }
        }
        else if (requester == FastAlgoDispatch)
        {
            if (broker->fastRideBool)
            { // Checks to see if time option was inputed for fast drive
                // multiplies wait time by 1000 since usleep is in microseconds
                usleep(broker->fastRideTime * MULTIPLE_FOR_SECONDS);
            }
        }
    }
    return NULL;
}