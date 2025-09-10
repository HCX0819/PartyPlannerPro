#include "event_booking.h"

// Simple function to get integer input safely
int getSafeInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(10000, '\n'); // remove leftover input
            return value;
        }
        else {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            cout << "Invalid number, please try again.\n";
        }
    }
}

void eventBookingMenu(vector<Guest>& guestList, vector<Event>& eventList) {
    cout << "--------------------------------------\n";
    cout << " Welcome to the Event Booking System! \n";
    cout << "--------------------------------------\n";

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Book a new Event\n";
        cout << "2. View all Events\n";
        cout << "3. Save Events to File\n";
        cout << "4. Load Events from File\n";
        cout << "5. Delete an Event\n";
        cout << "6. Update an Event\n";
        cout << "0. Return to Main Menu\n";

        choice = getSafeInt("Enter your choice: ");

        switch (choice) {
        case 1: bookEvent(eventList); break;
        case 2: viewEventBookings(eventList); break;
        case 3: {
            cout << "Events will be saved automatically when you return to main menu.\n";
            break;
        }
        case 4: {
            cout << "Events are loaded automatically when you start the program.\n";
            break;
        }
        case 5: deleteEvent(eventList); break;
        case 6: updateEvent(eventList); break;
        case 0:
            cout << "Returning to Main Menu...\n";
            break;
        default: cout << "Not a valid option, please choose again.\n";
        }
    } while (choice != 0);
}

// Add a new event
void bookEvent(vector<Event>& events) {
    Event e;

    cout << "\nBooking a new event...\n";
    cout << "Event Name: ";
    getline(cin, e.eventName);
    while (e.eventName.empty()) {
        cout << "Event name cannot be empty. Please enter again: ";
        getline(cin, e.eventName);
    }

    // get valid date and time
    do {
        cout << "Date (DD/MM/YYYY): ";
        getline(cin, e.date);
        while (!isValidDate(e.date)) {
            cout << "Invalid date. Please enter again (DD/MM/YYYY): ";
            getline(cin, e.date);
        }

        cout << "Time (HH:MM): ";
        getline(cin, e.time);
        while (!isValidTime(e.time)) {
            cout << "Invalid time. Please enter again (HH:MM): ";
            getline(cin, e.time);
        }

        // check conflict with existing events
        if (isEventConflict(events, e.date, e.time)) {
            char choice;
            cout << "Another event already exists at this date and time.\n";
            cout << "Do you want to re-enter? (Y/N): ";
            cin >> choice;
            cin.ignore();
            if (choice == 'Y' || choice == 'y') continue;
            else {
                cout << "Booking cancelled.\n";
                return;
            }
        }
        break;
    } while (true);

    // Location selection with fixed options + custom option
    int locChoice;
    cout << "\nChoose a Location:\n";
    cout << "1. Home\n";
    cout << "2. Restaurant\n";
    cout << "3. Hotel\n";
    cout << "4. Community Hall\n";
    cout << "5. Outdoor Park\n";
    cout << "6. Other (type your own)\n";

    locChoice = getSafeInt("Enter your choice (1-6): ");

    switch (locChoice) {
    case 1: e.location = "Home"; break;
    case 2: e.location = "Restaurant"; break;
    case 3: e.location = "Hotel"; break;
    case 4: e.location = "Community Hall"; break;
    case 5: e.location = "Outdoor Park"; break;
    case 6: {
        cout << "Enter custom location: ";
        getline(cin, e.location);
        while (e.location.empty()) {
            cout << "Location cannot be empty. Enter again: ";
            getline(cin, e.location);
        }
        break;
    }
    default:
        cout << "Invalid choice. Location set to 'Unspecified'.\n";
        e.location = "Unspecified";
    }

    events.push_back(e);
    cout << "Event booked successfully!\n";
}

// Show all events
void viewEventBookings(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events booked yet.\n";
        return;
    }

    cout << "\nYour booked events:\n";
    cout << left << setw(5) << "No."
        << setw(25) << "Event Name"
        << setw(12) << "Date"
        << setw(8) << "Time"
        << "Location\n";
    cout << string(60, '-') << "\n";

    for (size_t i = 0; i < events.size(); i++) {
        cout << left << setw(5) << (i + 1)
            << setw(25) << events[i].eventName
            << setw(12) << events[i].date
            << setw(8) << events[i].time
            << events[i].location << "\n";
    }
}

// Save all events into a text file
void saveEventsToFile(const vector<Event>& events, const string& currentUser) {
    string filename = currentUser + "_events.txt";
    
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error opening file to save.\n";
        return;
    }

    for (const auto& e : events) {
        outFile << e.eventName << "\n"
            << e.date << "\n"
            << e.time << "\n"
            << e.location << "\n";
    }
    outFile.close();
    cout << "Events saved to file.\n";
}

// Load events from file
void loadEventsFromFile(vector<Event>& events, const string& currentUser) {
    string filename = currentUser + "_events.txt";
    
    ifstream inFile(filename);
    if (!inFile) {
        cout << "No saved events found.\n";
        return;
    }

    events.clear();
    Event e;
    while (getline(inFile, e.eventName)) {
        getline(inFile, e.date);
        getline(inFile, e.time);
        getline(inFile, e.location);
        events.push_back(e);
    }
    inFile.close();
    cout << "Events loaded from file.\n";
}

// Delete an event
void deleteEvent(vector<Event>& events) {
    if (events.empty()) {
        cout << "There are no events to delete.\n";
        return;
    }

    viewEventBookings(events);
    int choice = getSafeInt("Enter the event number to delete (0 to cancel): ");

    if (choice == 0) {
        cout << "Delete cancelled.\n";
        return;
    }

    if (choice < 1 || choice >(int)events.size()) {
        cout << "Invalid number.\n";
        return;
    }

    cout << "Deleting: " << events[choice - 1].eventName << "...\n";
    events.erase(events.begin() + (choice - 1));
    cout << "Event deleted successfully.\n";
}

// Update an existing event
void updateEvent(vector<Event>& events) {
    if (events.empty()) {
        cout << "There are no events to update.\n";
        return;
    }

    viewEventBookings(events);
    int choice = getSafeInt("Enter the event number to update (0 to cancel): ");

    if (choice == 0) {
        cout << "Update cancelled.\n";
        return;
    }

    if (choice < 1 || choice >(int)events.size()) {
        cout << "Invalid number.\n";
        return;
    }

    Event& e = events[choice - 1];
    string input;

    cout << "Editing Event: " << e.eventName << "\n";

    cout << "New Event Name (leave empty to keep '" << e.eventName << "'): ";
    getline(cin, input);
    if (!input.empty()) e.eventName = input;

    cout << "New Date (DD/MM/YYYY) or leave empty to keep '" << e.date << "': ";
    getline(cin, input);
    if (!input.empty()) {
        while (!isValidDate(input)) {
            cout << "Invalid date. Enter again (DD/MM/YYYY): ";
            getline(cin, input);
        }
        e.date = input;
    }

    cout << "New Time (HH:MM) or leave empty to keep '" << e.time << "': ";
    getline(cin, input);
    if (!input.empty()) {
        while (!isValidTime(input)) {
            cout << "Invalid time. Enter again (HH:MM): ";
            getline(cin, input);
        }
        e.time = input;
    }

    cout << "New Location (leave empty to keep '" << e.location << "'): ";
    getline(cin, input);
    if (!input.empty()) e.location = input;

    cout << "Event updated successfully.\n";
}

// Check if date is valid (basic validation)
bool isValidDate(const string& date) {
    if (date.size() != 10 || date[2] != '/' || date[5] != '/') return false;

    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);

    if (!isdigit(dayStr[0]) || !isdigit(dayStr[1]) ||
        !isdigit(monthStr[0]) || !isdigit(monthStr[1]) ||
        !isdigit(yearStr[0]) || !isdigit(yearStr[1]) ||
        !isdigit(yearStr[2]) || !isdigit(yearStr[3])) return false;

    int day, month, year;
    try {
        day = stoi(dayStr);
        month = stoi(monthStr);
        year = stoi(yearStr);
    } catch (...) {
        return false; // Invalid date format
    }

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap) daysInMonth[1] = 29;

    if (day > daysInMonth[month - 1]) return false;

    return true;
}

// Check if time is valid (24-hour format)
bool isValidTime(const string& time) {
    if (time.size() != 5 || time[2] != ':') return false;

    string hourStr = time.substr(0, 2);
    string minuteStr = time.substr(3, 2);

    if (!isdigit(hourStr[0]) || !isdigit(hourStr[1]) ||
        !isdigit(minuteStr[0]) || !isdigit(minuteStr[1])) return false;

    int hour, minute;
    try {
        hour = stoi(hourStr);
        minute = stoi(minuteStr);
    } catch (...) {
        return false; // Invalid time format
    }

    return !(hour < 0 || hour > 23 || minute < 0 || minute > 59);
}

// Check if another event exists at the same date and time
bool isEventConflict(const vector<Event>& events, const string& date, const string& time) {
    for (const auto& e : events) {
        if (e.date == date && e.time == time) return true;
    }
    return false;
}