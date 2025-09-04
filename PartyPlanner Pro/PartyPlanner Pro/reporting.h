#ifndef REPORTING_H
#define REPORTING_H
#include "common.h"
#include "event_booking.h"

void generateSummary(const vector<Guest>& guestList, const vector<Event>& events);
void generateDetailedReport(const vector<Guest>& guestList);
void reportingMenu(const vector<Guest>& guestList, const vector<Event>& events);

// CSV Export Functions
void exportSummaryToCSV(const vector<Guest>& guestList, const vector<Event>& events);
void exportDetailedToCSV(const vector<Guest>& guestList);
void openFileInDefaultApp(const string& filename);

#endif
