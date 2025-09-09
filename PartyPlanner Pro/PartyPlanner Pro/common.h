#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <iomanip>
using namespace std;

// Forward declaration
struct Event;

// Guest structure
struct Guest {
    string name;
    string phone;
    string rsvp;   // "Yes", "No", "Not Responded"
    string food;   // food choice
};

// Global variables removed - now passed as parameters

#endif