#include <iostream>
#include "math.h"
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

struct Event {
    double time; // when event occurs. For arrival event, it's the time the packet arrives at the transmitter. For a departure event, it's the time when server is finished transmitting the packet.
    enum event_type { arrival, departure };
    event_type type;
    Event* next;
    Event* prev; 
};
 
double negative_exponentially_distributed_time(double rate) {
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
}

void initialize() {
    // TODO: initialize all data structures

    // counters for maintaining statistics
    total_time = 0;
    server_busy_count = 0;
    total_length = 0;
    packets_dropped = 0;

    length = 0; // number of packets in queue
    current_time = 0;

    // Create first arrival event
    Event *first = new Event;    
    double first_arrival_time = negative_exponentially_distributed_time(arrival_rate) + current_time; // event time for first arrival event
    first->time = first_arrival_time;
    first->type = Event::arrival;
    first->next = nullptr;
    first->prev = nullptr;
    
    // TODO: insert first arrival event into GEL
    
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