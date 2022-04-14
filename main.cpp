#include "broker.hpp"

int main(int argc, char *argv[]){

    Broker *broker = new Broker;
    broker->ridesQueue = new queue<int>;
    broker->maxRides = MAX_RIDES_DEFAULT; //Sets Max Rides to Default 120
    //Sets wait time booleans to false at beginning
    broker->costSaveRideBool = false;
    broker->fastRideBool = false;
    broker->produceRideHumanBool = false;
    broker->produceRideRoboBool = false;

    //Sets ProducerID to 0 at beginning
    broker->producerId = DEFAULT;
    //Sets ConsumerID to 0 at beginning
    broker->consumerId = DEFAULT;
    //Sets consumed to 0 at beginning
    broker->consumed = DEFAULT;
    //Sets produced to 0 at begining
    broker->produced = DEFAULT;
    
    //Initalizes and populates consumedTotals 2D int array to 0
    broker->consumedTotals = new int*[NUMBER_OF_TYPES];
    for(int i = 0; i < NUMBER_OF_TYPES; i++){
        broker->inRequestQueue[i] = DEFAULT;
        broker->Produced[i] = DEFAULT;
        broker->consumedTotals[i] = new int[NUMBER_OF_TYPES];
        for(int j = 0; j < NUMBER_OF_TYPES; ++j){
            broker->consumedTotals[i][j] = DEFAULT;
        }
    }

    //Starts reading in optional arguments
    int Option;
    while((Option = getopt(argc, argv, "n:c:f:h:a:")) != -1){
        switch (Option){
            case 'n':
                //User set limit for max amount of broker
                broker->maxRides = atoi(optarg);
                break;
            case 'c':
                //Number of milliseconds that cost-saving 
                //dispatcher requires dispatching a request
                //Assigns variable and sets bool to true for wait time exists
                broker->costSaveRideBool = true;
                broker->costSaveTime = atoi(optarg);
                break;
            case 'f':
                //Number of milliseconds that fast-matching dispatcher
                //requires dispatching a request
                //Assigns variable and sets bool to true for wait time exists
                broker->fastRideBool = true;
                broker->fastRideTime = atoi(optarg);
                break;
            case 'h':
                //Number of milliseconds required to produce a ride 
                //request for a human driver
                //Assigns variable and sets bool to true for wait time exists
                broker->produceRideHumanBool =true;
                broker->produceRideHuman = atoi(optarg);
                break;
            case 'a':
                //number of milliseconds required to produce a ride request for
                //an autonomous car
                //Assigns variable and sets bool to true for wait time exists
                broker->produceRideRoboBool =true;
                broker->produceRideRobo = atoi(optarg);
                break;
            default:
                break;
        }
    }

    //Initalizes all the semaphores within Broker struct
    sem_init(&broker->mutex, 0,1); //Buffer Access key
    sem_init(&broker->unUsedRides, 0 ,0); //Available ride requests
    sem_init(&broker->availableSlots, 0, RIDE_REQUEST_MAX_SLOTS); //Max ammount of ride requests in a queue
    sem_init(&broker->maxHumanDrivers, 0, MAX_REQUEST_HUMAN_DRIVERS); //Max amount of ride requests for human drivers to be produced
    //Declares the threads for each producer and consumer
    pthread_t HDR, RDR, CostAD, FastAD;

    //Creates the threads and runs them
    pthread_create(&HDR, NULL, Producer, broker);
    pthread_create(&RDR, NULL, Producer, broker);
    pthread_create(&CostAD, NULL, Consumer, broker);
    pthread_create(&FastAD, NULL, Consumer, broker);

    //Joins the threads
    pthread_join(HDR, NULL);
    pthread_join(RDR, NULL);
    pthread_join(CostAD, NULL);
    pthread_join(FastAD, NULL);

    //Prints the report from io
    io_production_report(broker->Produced, broker->consumedTotals);

    //Destroy semaphores to avoid memory leaks
    sem_destroy(&broker->mutex);
    sem_destroy(&broker->unUsedRides);
    sem_destroy(&broker->availableSlots);
    sem_destroy(&broker->maxHumanDrivers);

    return 0;
}