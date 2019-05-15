#include <iostream>
#include <cmath>
#include <list>
#include <queue>
using namespace std;

// TODO: MAXIMUM BUFFER

struct Event {
    double event_time; // when event occurs. For arrival event, it's the time the packet arrives at the transmitter. For a departure event, it's the time when server is finished transmitting the packet.
    double service_time;
    enum event_type { arrival, departure };
    event_type type;
    Event* next;
    Event* prev; 
};

/* global variables for processing */
int length;
double current_time;
double service_rate; // mu
double arrival_rate; // lambda
list <Event*> GEL; // GEL (Global Event List) sorted in increasing order of time
queue <Event*> buffer; // models the buffer

/* global variables for statistics */
double total_time;
double server_busy_count;
int total_length;
int packets_dropped;

/* output statistics */
double mean_server_utilization;
double mean_queue_length;
double number_packets_dropped;
 
double negative_exponentially_distributed_time(double rate) {
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
}

void initialize() {

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
    double first_service_time = negative_exponentially_distributed_time(service_rate);
    first->event_time = first_arrival_time;
    first->service_time = first_service_time;
    first->type = Event::arrival;
    first->next = nullptr;
    first->prev = nullptr;
    // TODO: may need service time somewhere in between
    
    GEL.push_front(first); // insert first arrival event to GEL
    
}

void process_arrival_event(Event *ev) {
    // TODO: Set current time to be the event time
    // TODO: Schedule next arrival event
    // TODO: Process arrival event
}

int main() {

    cout << "Enter the service rate (packet/second): ";
    cin >> service_rate;

    cout << "Enter the arrival rate (packets/second): ";
    cin >> arrival_rate;
    
    initialize();

    for (int i = 0; i < 100000; ++i) {
        // 1. get first event from GEL
        if (GEL.empty()) {
            break;
        }
        Event *ev = GEL.front(); // get front because first element needs to be the next event
        GEL.pop_front();
        // 2. if the first event is an arrival event then process-arrival-event
        if (ev->type == Event::arrival) {
            process_arrival_event(ev);
        }
        // 3. Otherwise, it must be a departure event and hence process-service-completion
    }
    

   // output statistics

}