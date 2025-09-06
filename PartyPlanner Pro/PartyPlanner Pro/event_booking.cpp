#include "event_booking.h"

// ==================== SAFE INPUT HANDLER ====================
int getSafeInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(10000, '\n'); // discard extra input
            return value;
        }
        else {
            cin.clear(); // clear error state
            cin.ignore(10000, '\n'); // discard invalid input
            cout << "Oops, that doesn't look like a number. Please try again.\n";
        }
    }
}

// ==================== EVENT BOOKING MENU ====================
void eventBookingMenu(vector<Guest>& guestList) {
    // Use global event list (declared in common.h)

    cout << "--------------------------------------\n";
    cout << " Welcome to the Event Booking System! \n";
    cout << "--------------------------------------\n";

    int choice;
    do {
        cout << "\nWhat would you like to do?\n";
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
        case 3: saveEventsToFile(eventList); break;
        case 4: loadEventsFromFile(eventList); break;
        case 5: deleteEvent(eventList); break;
        case 6: updateEvent(eventList); break;
        case 0:
            cout << "Returning to Main Menu...\n";
            saveEventsToFile(eventList); // Auto-save before returning
            break;
        default: cout << "That's not a valid option. Please try again.\n";
        }
    } while (choice != 0);
}

// ==================== BOOK EVENT ====================
void bookEvent(vector<Event>& events) {
    Event e;
    cin.ignore(); // clear buffer

    cout << "\nLet's book a new event!\n";
    cout << "Event Name: ";
    getline(cin, e.eventName);
    while (e.eventName.empty()) {
        cout << "Event name cannot be empty. Please enter again: ";
        getline(cin, e.eventName);
    }

    // Date & Time with validation and conflict check
    do {
        cout << "Date (DD/MM/YYYY): ";
        getline(cin, e.date);
        while (!isValidDate(e.date)) {
            cout << "That date doesn't look right. Please enter again (DD/MM/YYYY): ";
            getline(cin, e.date);
        }

        cout << "Time (HH:MM): ";
        getline(cin, e.time);
        while (!isValidTime(e.time)) {
            cout << "That time is invalid. Please enter again (HH:MM): ";
            getline(cin, e.time);
        }

        if (isEventConflict(events, e.date, e.time)) {
            char choice;
            cout << "There's already an event booked on " << e.date << " at " << e.time << ".\n";
            cout << "Would you like to re-enter the date/time? (Y/N): ";
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

    cout << "Location: ";
    getline(cin, e.location);
    while (e.location.empty()) {
        cout << "Location cannot be empty. Please enter again: ";
        getline(cin, e.location);
    }

    events.push_back(e);
    cout << "Great! Your event has been booked successfully.\n";
}

// ==================== VIEW EVENTS ====================
void viewEventBookings(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events booked yet.\n";
        return;
    }

    cout << "\nHere are your events:\n";
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

// ==================== SAVE TO FILE ====================
void saveEventsToFile(const vector<Event>& events) {
    extern string currentUser;
    string filename = currentUser.empty() ? "events.txt" : currentUser + "_events.txt";
    
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Couldn't open file for writing.\n";
        return;
    }

    for (const auto& e : events) {
        outFile << e.eventName << "\n"
            << e.date << "\n"
            << e.time << "\n"
            << e.location << "\n";
    }
    outFile.close();
    cout << "All events have been saved to file.\n";
}

// ==================== LOAD FROM FILE ====================
void loadEventsFromFile(vector<Event>& events) {
    extern string currentUser;
    string filename = currentUser.empty() ? "events.txt" : currentUser + "_events.txt";
    
    ifstream inFile(filename);
    if (!inFile) {
        cout << "No saved events were found.\n";
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
    cout << "Events have been loaded from file.\n";
}

// ==================== DELETE EVENT ====================
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
        cout << "That's not a valid number.\n";
        return;
    }

    cout << "Deleting: " << events[choice - 1].eventName << "...\n";
    events.erase(events.begin() + (choice - 1));
    cout << "The event has been removed successfully.\n";
}

// ==================== UPDATE EVENT ====================
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
        cout << "That's not a valid number.\n";
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
            cout << "That date doesn't look right. Enter again (DD/MM/YYYY): ";
            getline(cin, input);
        }
        e.date = input;
    }

    cout << "New Time (HH:MM) or leave empty to keep '" << e.time << "': ";
    getline(cin, input);
    if (!input.empty()) {
        while (!isValidTime(input)) {
            cout << "That time doesn't look right. Enter again (HH:MM): ";
            getline(cin, input);
        }
        e.time = input;
    }

    cout << "New Location (leave empty to keep '" << e.location << "'): ";
    getline(cin, input);
    if (!input.empty()) e.location = input;

    cout << "Your event has been updated successfully.\n";
}

// ==================== VALIDATION ====================
bool isValidDate(const string& date) {
    if (date.size() != 10 || date[2] != '/' || date[5] != '/') return false;

    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);

    if (!isdigit(dayStr[0]) || !isdigit(dayStr[1]) ||
        !isdigit(monthStr[0]) || !isdigit(monthStr[1]) ||
        !isdigit(yearStr[0]) || !isdigit(yearStr[1]) ||
        !isdigit(yearStr[2]) || !isdigit(yearStr[3])) return false;

    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap) daysInMonth[1] = 29;

    if (day > daysInMonth[month - 1]) return false;

    return true;
}

bool isValidTime(const string& time) {
    if (time.size() != 5 || time[2] != ':') return false;

    string hourStr = time.substr(0, 2);
    string minuteStr = time.substr(3, 2);

    if (!isdigit(hourStr[0]) || !isdigit(hourStr[1]) ||
        !isdigit(minuteStr[0]) || !isdigit(minuteStr[1])) return false;

    int hour = stoi(hourStr);
    int minute = stoi(minuteStr);

    return !(hour < 0 || hour > 23 || minute < 0 || minute > 59);
}

bool isEventConflict(const vector<Event>& events, const string& date, const string& time) {
    for (const auto& e : events) {
        if (e.date == date && e.time == time) return true;
    }
    return false;
}