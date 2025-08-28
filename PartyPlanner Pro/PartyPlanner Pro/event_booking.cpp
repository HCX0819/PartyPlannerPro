#include "event_booking.h"

void saveToFile(const vector<Guest>& guestList) {
    ofstream file("guests.txt");
    if (!file) {
        cout << "Error saving file.\n";
        return;
    }
    for (const auto& g : guestList) {
        file << g.name << "," << g.phone << "," << g.rsvp << "," << g.food << "\n";
    }
    file.close();
    cout << "Data saved to guests.txt\n";
}

void loadFromFile(vector<Guest>& guestList) {
    ifstream file("guests.txt");
    if (!file) {
        cout << "No file found.\n";
        return;
    }
    guestList.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Guest g;
        getline(ss, g.name, ',');
        getline(ss, g.phone, ',');
        getline(ss, g.rsvp, ',');
        getline(ss, g.food, ',');
        guestList.push_back(g);
    }
    file.close();
    cout << "Data loaded from guests.txt\n";
}

void bookEvent() {
    string name, date, time;
    cin.ignore();
    cout << "Enter event name: ";
    getline(cin, name);
    cout << "Enter event date: ";
    getline(cin, date);
    cout << "Enter event time: ";
    getline(cin, time);
    cout << "Event booked: " << name << " on " << date << " at " << time << "\n";
}

void eventBookingMenu(vector<Guest>& guestList) {
    int choice;
    do {
        cout << "\n--- Event Booking ---\n";
        cout << "1. Book Event\n";
        cout << "2. Save Guest Data\n";
        cout << "3. Load Guest Data\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: bookEvent(); break;
        case 2: saveToFile(guestList); break;
        case 3: loadFromFile(guestList); break;
        case 4: break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 4);
}