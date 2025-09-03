#ifndef REPORTING_H
#define REPORTING_H
#include "common.h"

void generateSummary(const vector<Guest>& guestList);
void generateDetailedReport(const vector<Guest>& guestList);
void reportingMenu(const vector<Guest>& guestList);

#endif
