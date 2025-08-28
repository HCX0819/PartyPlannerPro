#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// Guest structure
struct Guest {
    string name;
    string phone;
    string rsvp;   // "Yes", "No", "Not Responded"
    string food;   // food choice
};

// Global guest list (shared between modules via reference)
extern vector<Guest> guestList;

#endif