#include <iostream>
using namespace std;

// TODO: MAXIMUM BUFFER

/* global variables for processing */
int length;
double current_time;
double service_rate; // mu
double arrival_rate; // lambda

/* global variables for statistics */
double total_time;
double server_busy_count;
int total_length;
int packets_dropped;

/* output statistics */
double mean_server_utilization;
double mean_queue_length;
double number_packets_dropped;

void initialize() {
    cout << "initialize" << endl;
    // TODO: initialize all data structures

    // counters for maintaining statistics
    total_time = 0;
    server_busy_count = 0;
    total_length = 0;
    packets_dropped = 0;

    length = 0; // number of packets in queue
    current_time = 0;

    // TODO: create first arrival event
    // TODO: insert first arrival event into GEL
    // TODO: event time of first arrival = obtained by adding a randomly generated inter-arrival time to the current time (0)
}

int main() {

    cout << "Enter the service rate (packet/second): ";
    cin >> service_rate;

    cout << "Enter the arrival rate (packets/second): ";
    cin >> arrival_rate;
    
    initialize();

    /*
        for i = 0; i < 100000; i++
            1. get first event from GEL
            2. if the event is an arrival then process-arrival-event
            3. otherwise it must be a departure event and hence process-service-completion 
    */

   // output statistics

}