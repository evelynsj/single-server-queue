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
queue <Event*> buffer; // models the buffer
Event* GELhead;
Event* GELtail;

/* global variables for statistics */
double total_time;
double server_busy_count;
int total_length;
int packets_dropped;

/* output statistics */
double mean_server_utilization;
double mean_queue_length;
double number_packets_dropped;

void insert(Event *ev);
void iterate();
 
double neg_exp_time(double rate) {
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
}

void create_arrival(double ev_time, double serv_time) {
    Event *ev = new Event;
    ev->event_time = ev_time;
    ev->service_time = serv_time;
    ev->type = Event::arrival;
    insert(ev);
}

void initialize() {

    // initialize head and tail
    GELhead = nullptr;
    GELtail = nullptr; // TODO: do we need tail????

    // counters for maintaining statistics
    total_time = 0;
    server_busy_count = 0;
    total_length = 0;
    packets_dropped = 0;

    length = 0; // number of packets in queue
    current_time = 0;

    // Create first arrival event
    double first_arrival_time = neg_exp_time(arrival_rate) + current_time; // event time for first arrival event
    double first_service_time = neg_exp_time(service_rate);
    create_arrival(first_arrival_time, first_service_time);
    
    cout << "Curr time is " << current_time << endl;
    iterate();
    
}

void insert(Event* event) {
    // if head is nullptr
    if (GELhead == nullptr) {
        GELhead = event;
        GELhead->next = nullptr; // TODO: is this where we assign next and prev
        GELhead->prev = nullptr; 
    }
    // TODO: insert sorted
}

void delete_head(Event *event) {
    if (GELhead == event) { // if node is head
        GELhead = event->next;
    }
    // TODO: if node is tail
    // TODO: if node is middle
}

void iterate() {
    Event *curr = GELhead;
    while (curr != nullptr) {
        if (curr->type == Event::arrival) {
            cout << "Arrival ";
        } else {
            cout << "Departure ";
        }
        cout << curr->event_time << endl;
        curr = curr->next;
    }
    cout << "done iterating" << endl;
}

// void process_arrival_event(Event *ev) {
//     current_time = ev->event_time; // set current time to be event time
//     // Schedule next arrival event
//         double next_event_time = current_time + negative_exponentially_distributed_time(arrival_rate);
//         double next_service_time = negative_exponentially_distributed_time(service_rate);

//         Event *next_ev = new Event; // Create new arrival event
//         next_ev->event_time = next_event_time;
//         next_ev->service_time = next_service_time;
//         next_ev->type = Event::arrival;

//         insert(next_ev); // Insert event into GEL. Might need to order

//     // TODO: Process arrival event
//         // TODO: If server is free (length == 0)
//              // TODO: Schedule a departure event:
//                 // TODO: Get service time
//                 // TODO: Create a departure event (time = curr time + service time)
//                 // TODO: Insert event into GEL (make sure it's sorted)
//         // TODO: if server is not free (length > 0)
//             // TODO: If queue is not full (length - 1 < MAXBUFFER), put packet into queue
//             // TODO: if queue is full, drop packet and RECORD
//             // TODO: Since this is a new arrival event, increment length
//             // TODO: Update STATS

// }

int main() {

    cout << "Enter the service rate (packet/second): ";
    cin >> service_rate;

    cout << "Enter the arrival rate (packets/second): ";
    cin >> arrival_rate;
    
    initialize();

    // for (int i = 0; i < 2; ++i) { // 100000
    //     // 1. get first event from GEL
    //     if (GELhead == nullptr) { // TODO: TRY USING A CLASS SO CAN KEEP SIZE (PRIVATE)
    //         break;
    //     }
    //     Event *ev = GELhead; // get front because first element needs to be the next event
    //     delete_head(GELhead); // delete front
    //     // 2. if the first event is an arrival event then process-arrival-event
    //     if (ev->type == Event::arrival) {
    //         process_arrival_event(ev);
    //     }
    //     // 3. Otherwise, it must be a departure event and hence process-service-completion
    // }
    

   // output statistics

}