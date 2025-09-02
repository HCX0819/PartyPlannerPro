#ifndef EVENT_BOOKING_H
#define EVENT_BOOKING_H
#include "common.h"

// Event structure
struct Event {
    string eventName;   // Name of the event
    string date;        // Date in DD/MM/YYYY format
    string time;        // Time in HH:MM format
    string location;    // Event location
};

// Function prototypes
void eventBookingMenu(vector<Guest>& guestList);
void bookEvent(vector<Event>& events);
void viewEventBookings(const vector<Event>& events);
void saveEventsToFile(const vector<Event>& events);
void loadEventsFromFile(vector<Event>& events);
void deleteEvent(vector<Event>& events);
void updateEvent(vector<Event>& events);

bool isValidDate(const string& date);
bool isValidTime(const string& time);
bool isEventConflict(const vector<Event>& events, const string& date, const string& time);
int getSafeInt(const string& prompt);

#endif