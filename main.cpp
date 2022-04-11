#include "producers.h"
#include "consumers.h"
#include "main.h"

int main(int argc, char *argv[]){

    //Variable initalization
    buffer *rides = new buffer;
    rides->ridesQueue = new queue<Request*>;
    rides-> productionLimit = MAX_RIDES;
    rides->consumerTime = DEFAULT;
    rides->consumerTimeBool = false;
    rides->matchingDispatcher = DEFAULT;
    rides->matchingDispatcherBool = false;
    rides->produceRideHuman = DEFAULT;
    rides->produceRideHumanBool = false;
    rides->produceRideAutonomus = DEFAULT;
    rides->produceRideAutonomusBool = false;
    rides->producerId = 0;
    rides->consumed = 0;


    sem_init(&rides->mutex, 0,1); //MUTUAL BUFFER ACCESS
    sem_init(&rides->unconsumed, 0 ,0); //Available ride requests
    sem_init(&rides->availableSlots, 0, RIDE_REQUEST_MAX_SLOTS); //Max ammount of ride requests available at start
    sem_init(&rides->maxHumanDrivers, 0, MAX_REQUEST_HUMAN_DRIVERS); //Max amount of rides reached to stop
    sem_init(&rides->limit, 0, MAX_RIDES); //Max amount of human drivers allowed in ride queue


    int Option;
    while((Option = getopt(argc, argv, "n:c:f:h:a:")) != -1){
        switch (Option){
            case 'n':
                //User set limit
                printf("n: %s\n",optarg);
                rides->productionLimit = atoi(optarg);
                break;
            case 'c':
                //Number of milliseconds that cost-saving 
                //dispatcher requires dispatching a request
                printf("c: %s\n",optarg);
                rides->consumerTime = atoi(optarg);
                break;
            case 'f':
                //Number of milliseconds that fast-matching dispatcher
                //requires dispatching a request
                printf("f: %s\n",optarg);
                rides->matchingDispatcher = atoi(optarg);
                break;
            case 'h':
                //number of milliseconds required to produce a ride 
                //request for a human driver
                printf("h: %s\n",optarg);
                rides->produceRideHuman = atoi(optarg);
                break;
            case 'a':
                //number of milliseconds required to produce a ride request for
                //an autonomous car
                printf("a: %s\n",optarg);
                rides->produceRideAutonomus = atoi(optarg);
                break;
            default:
                break;
        }
    }

    //pthread_t HDR, RDR, CostAD, FastAD;





    return 0;
}

Request::Request(int id){
    request_id = id;
}