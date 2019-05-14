#include <iostream>
using namespace std;

/* global variables for statistics */
double total_time;
double server_busy_count;
int total_length;
int packets_dropped;

/* output statistics */
double mean_server_utilization;
double mean_queue_length;
double number_packets_dropped;

int main() {

    // initialization

    /*
        for i = 0; i < 100000; i++
            1. get first event from GEL
            2. if the event is an arrival then process-arrival-event
            3. otherwise it must be a departure evetn and hence process-service-completion 
    */

   // output statistics

}