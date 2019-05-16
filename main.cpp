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
int GELsize;

/* global variables for statistics */
double total_time;
double server_busy_time;
int total_length; // TODO: FIGURE OUT WHERE THE TOTAL LENGTH GOES
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

void create_departure(double ev_time, double serv_time) {
    Event *ev = new Event;
    ev->event_time = ev_time;
    ev->service_time = serv_time;
    ev->type = Event::departure;
    insert(ev);
}

void initialize() {

    // initialize head and tail
    GELhead = nullptr;
    GELtail = nullptr; // TODO: do we need tail????
    GELsize = 0;

    // counters for maintaining statistics
    total_time = 0;
    server_busy_time = 0;
    total_length = 0;
    packets_dropped = 0;

    length = 0; // number of packets in queue
    current_time = 0;

    // Create first arrival event
    double first_arrival_time = neg_exp_time(arrival_rate) + current_time; // event time for first arrival event
    double first_service_time = neg_exp_time(service_rate);
    create_arrival(first_arrival_time, first_service_time);

    iterate();
    
}

void insert(Event* event) { // insert to GEL
    // if head is nullptr
    if (GELhead == nullptr) {
        GELhead = event;
        GELhead->next = nullptr; // TODO: is this where we assign next and prev
        GELhead->prev = nullptr; 
    }
    else { // TODO: insert sorted
        cout << "insert sorted" << endl;
        if (event->event_time < GELhead->event_time) { // insert in front of head
            event->next = GELhead;
            event->prev = nullptr;
            GELhead->prev = event;
            GELhead = event;
        }
        // TODO: if insert in the middle
        // TODO: if insert at the end
    }
    GELsize++;
}

void delete_head() {
    cout << "Delete head" << endl;
    GELhead = GELhead->next;
    GELsize--;
    // TODO: if node is tail
    // TODO: if node is middle
}

void iterate() {
    cout << "***********" << endl;
    cout << "start iterating" << endl;
    cout << "GELsize is " << GELsize << endl;
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
    cout << "************" << endl;
}

void process_arrival_event(Event *curr_ev) {
    current_time = curr_ev->event_time; // set current time to be event time

    // Schedule next arrival event
    double next_event_time = current_time + neg_exp_time(arrival_rate);
    double next_service_time = neg_exp_time(service_rate);
    create_arrival(next_event_time, next_service_time);
    iterate();

    // TODO: Process arrival event
        if (length == 0) { // Server is free
            // Schedule a departure event:
                double processing_service_time = curr_ev->service_time;
                double departure_event_time = current_time + processing_service_time; // Create a departure event (time = curr time + service time)
                create_departure(departure_event_time, processing_service_time);
                iterate();
        }
        
        // TODO: if server is not free (length > 0)
            // TODO: If queue is not full (length - 1 < MAXBUFFER), put packet into queue
            // TODO: if queue is full, drop packet and RECORD
            // TODO: Since this is a new arrival event, increment length
            // TODO: Update STATS

}

void process_departure_event(Event* ev) {
    cout << "process departure" << endl;
    current_time = ev->event_time;
    double processing_service_time = ev->service_time;
    cout << current_time << endl;
    // TODO: MIGHT NEED TO UPDATE TOTAL LENGTH
    server_busy_time += processing_service_time;
    length--;
    // TODO: If queue is empty (length ==0) do nothing
    // TODO: If queue is not empty (length > 0) do:
        // TODO: Dequeue first packet from buffer
        // TODO: Create new departure event for a time which si current time + service time
        // TODO: Insert event at the right place in GEL

}

int main() {

    cout << "Enter the service rate (packet/second): ";
    cin >> service_rate;

    cout << "Enter the arrival rate (packets/second): ";
    cin >> arrival_rate;
    
    initialize();

    for (int i = 0; i < 2; ++i) { // 100000
        cout << "Iteration " << i << endl;
        // 1. get first event from GEL
        if (GELsize == 0) { // TODO: TRY USING A CLASS SO CAN KEEP SIZE (PRIVATE)
            break;
        }
        Event *ev = GELhead; // get front because first element needs to be the next event
        delete_head(); // delete front
        iterate();
        
        // 2. if the first event is an arrival event then process-arrival-event
        if (ev->type == Event::arrival) {
            process_arrival_event(ev);
        } else { // 3. Otherwise, it must be a departure event and hence process-service-completion
            process_departure_event(ev);
        }
    }
    

   // output statistics

}