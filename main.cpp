#include "broker.hpp"

int main(int argc, char *argv[]){

    //Variable initalization for all varibles in struct buffer
    buffer *rides = new buffer;
    rides->ridesQueue = new queue<int>;
    rides->maxRides = MAX_RIDES_DEFAULT; //Sets Max Rides to Default 120
    //Sets wait time booleans to false ate beginning
    rides->costSaveRideBool = false;
    rides->fastRideBool = false;
    rides->produceRideHumanBool = false;
    rides->produceRideRoboBool = false;


    //Sets ProducerID to 0 at beginning
    rides->producerId = DEFAULT;
    //Sets consumed to 0 at beginning
    rides->consumed = DEFAULT;
    
    //Initalizes and populates consumedTotals 2D int array to 0
    rides->consumedTotals = new int*[NUMBER_OF_TYPES];
    for(int i = 0; i < NUMBER_OF_TYPES; i++){
        rides->inRequestQueue[i] = DEFAULT;
        rides->Produced[i] = DEFAULT;
        rides->consumedTotals[i] = new int[NUMBER_OF_TYPES];
        for(int j = 0; j < NUMBER_OF_TYPES; ++j){
            rides->consumedTotals[i][j] = DEFAULT;
        }
    }

    //Starts reading in optional arguments
    int Option;
    while((Option = getopt(argc, argv, "n:c:f:h:a:")) != -1){
        switch (Option){
            case 'n':
                //User set limit for max amount of rides
                rides->maxRides = atoi(optarg);
                break;
            case 'c':
                //Number of milliseconds that cost-saving 
                //dispatcher requires dispatching a request
                //Assigns variable and sets bool to true for wait time exists
                rides->costSaveRideBool = true;
                rides->costSaveTime = atoi(optarg);
                break;
            case 'f':
                //Number of milliseconds that fast-matching dispatcher
                //requires dispatching a request
                //Assigns variable and sets bool to true for wait time exists
                rides->fastRideBool = true;
                rides->fastRideTime = atoi(optarg);
                break;
            case 'h':
                //Number of milliseconds required to produce a ride 
                //request for a human driver
                //Assigns variable and sets bool to true for wait time exists
                rides->produceRideHumanBool =true;
                rides->produceRideHuman = atoi(optarg);
                break;
            case 'a':
                //number of milliseconds required to produce a ride request for
                //an autonomous car
                //Assigns variable and sets bool to true for wait time exists
                rides->produceRideRoboBool =true;
                rides->produceRideRobo = atoi(optarg);
                break;
            default:
                break;
        }
    }

    //Initalizes all the semaphores within buffer struct
    sem_init(&rides->mutex, 0,1); //Buffer Access key
    sem_init(&rides->unconsumed, 0 ,0); //Available ride requests
    sem_init(&rides->availableSlots, 0, RIDE_REQUEST_MAX_SLOTS); //Max ammount of ride requests in a queue
    sem_init(&rides->maxHumanDrivers, 0, MAX_REQUEST_HUMAN_DRIVERS); //Max amount of ride requests for human drivers to be produced
    sem_init(&rides->limit, 0, rides->maxRides); //max amount of rides before stopping

    //Declares the threads for each producer and consumer
    pthread_t HDR, RDR, CostAD, FastAD;

    //Creates the threads and runs them
    pthread_create(&HDR, NULL, Producer, rides);
    pthread_create(&RDR, NULL, Producer, rides);
    pthread_create(&CostAD, NULL, Consumer, rides);
    pthread_create(&FastAD, NULL, Consumer, rides);

    //Joins the threads
    pthread_join(HDR, NULL);
    pthread_join(RDR, NULL);
    pthread_join(CostAD, NULL);
    pthread_join(FastAD, NULL);

    //Prints the report from io
    io_production_report(rides->Produced, rides->consumedTotals);

    //Destroy semaphores to avoid memory leaks
    sem_destroy(&rides->mutex);
    sem_destroy(&rides->unconsumed);
    sem_destroy(&rides->availableSlots);
    sem_destroy(&rides->maxHumanDrivers);
    sem_destroy(&rides->limit);

    return 0;
}