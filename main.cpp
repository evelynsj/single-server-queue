#include <iostream>
#include <cmath>
#include <list>
#include <queue>
using namespace std;

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
int MAXBUFFER;
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
 
double neg_exp_time(double rate) {
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
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

void insert(Event* event) { // insert to GEL
    // if head is nullptr
    if (GELhead == nullptr) {
        GELhead = event;
        GELhead->next = nullptr;
        GELhead->prev = nullptr; 
    }
    else {
        if (event->event_time < GELhead->event_time) { // insert in front of head
            event->next = GELhead;
            event->prev = nullptr;
            GELhead->prev = event;
            GELhead = event;
        }
        else {
            Event *curr = GELhead;
            Event *prev = nullptr;
            while (curr) {
                if (event->event_time < curr->event_time) { // Insert in the middle
                    prev = curr->prev;
                    prev->next = event;
                    event->prev = prev;
                    event->next = curr;
                    curr->prev = event;
                    break;
                }
                if (curr->next == nullptr && event->event_time > curr->event_time) { // Insert at the end
                    curr->next = event;
                    event->prev = curr;
                    event->next = nullptr;
                    break;
                }
                curr = curr->next;
            }
        }
    }
    GELsize++;
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
    GELtail = nullptr;
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
    
}

void delete_head() {
    GELhead = GELhead->next;
    GELsize--;
}

void process_arrival_event(Event *curr_ev) {
    current_time = curr_ev->event_time; // set current time to be event time
    // Schedule next arrival event
    double next_event_time = current_time + neg_exp_time(arrival_rate);
    double next_service_time = neg_exp_time(service_rate);
    create_arrival(next_event_time, next_service_time);

    // Process arrival event
    if (length == 0) { // Server is free
        // Schedule a departure event:
        double processing_service_time = curr_ev->service_time;
        double departure_event_time = current_time + processing_service_time; // Create a departure event (time = curr time + service time)
        create_departure(departure_event_time, processing_service_time);
        length++;
        total_length++;
    }
    else if (length > 0) { // If server is not free
        if (length - 1 < MAXBUFFER) { // if queue is not full, put packet into queue
            buffer.push(curr_ev);
            length++; // Since this is a new arrival event, increment length
            total_length++;
        }
        else { // if queue is full, drop packet and RECORD
            packets_dropped++;
        }
    }
}

void process_departure_event(Event* curr_ev) {
    current_time = curr_ev->event_time;
    double processing_service_time = curr_ev->service_time;
    server_busy_time += processing_service_time;
    --length;
    if (length == 0) { // do nothing if queue is empty
        return;
    }
    else if (length > 0) { // if queue is not empty
        Event *ev = buffer.front(); // Dequeue first packet from buffer
        buffer.pop();
        double next_event_time = current_time + ev->service_time; // Create departure event
        double next_service_time = ev->service_time;
        create_departure(next_event_time, next_service_time);
    }
}

void output_statistics() {
    total_time = current_time;
    mean_server_utilization = server_busy_time / total_time;
    cout << "Mean server utilization: " << mean_server_utilization << endl;
    mean_queue_length = total_length / total_time;
    cout << "Mean queue length: " << mean_queue_length << endl;
    number_packets_dropped = packets_dropped;
    cout << "Number of packets dropped: " << number_packets_dropped << endl;
}

int main() {

    cout << "Enter the service rate (packet/second): ";
    cin >> service_rate;

    cout << "Enter the arrival rate (packets/second): ";
    cin >> arrival_rate;

    cout << "Enter the maximum buffer: ";
    cin >> MAXBUFFER;
    
    initialize();

    for (int i = 0; i < 100000; ++i) { // 100000
        if (GELsize == 0) {
            break;
        }
        // 1. get first event from GEL
        Event *ev = GELhead; 
        delete_head(); // delete front
        
        // 2. if the first event is an arrival event then process-arrival-event
        if (ev->type == Event::arrival) {
            process_arrival_event(ev);
        } else { // 3. Otherwise, it must be a departure event and hence process-service-completion
            process_departure_event(ev);
        }

    }
    
   output_statistics();

}