#include "common.h"
#include "guest_management.h"
#include "food_selection.h"
#include "reporting.h"
#include "login.h"
#include "event_booking.h"
#include <fstream>
#include <sstream>
#include <limits>

vector<Guest> guestList;  // define global variable
vector<Event> eventList;  // define global event list
string currentUser = "";  // Current logged in user

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    // Authentication loop: allow logout back to login screen
    while (true) {
        if (!authenticationMenu()) {
            cout << "Exiting program...\n";
            return 0;
        }
        // Ensure clean state before loading user-specific data
        guestList.clear();
        eventList.clear();
        // Load user-specific data
        loadEventsFromFile(eventList);
        // Load user-specific guest data
        extern string currentUser;
        string guestFilename = currentUser.empty() ? "guests.txt" : currentUser + "_guests.txt";
        ifstream guestFile(guestFilename);
        if (guestFile.is_open()) {
            guestList.clear();
            string line;
            while (getline(guestFile, line)) {
                if (!line.empty()) {
                    stringstream ss(line);
                    Guest g;
                    getline(ss, g.name, ',');
                    getline(ss, g.phone, ',');
                    getline(ss, g.rsvp, ',');
                    getline(ss, g.food, ',');
                    guestList.push_back(g);
                }
            }
            guestFile.close();
            cout << "Loaded " << guestList.size() << " guests for user: " << currentUser << "\n";
        } else {
            guestList.clear();
            cout << "No guest data found for user: " << currentUser << "\n";
        }
        
        int choice;
        bool shouldLogout = false;
        do {
            cout << "\n===== PartyPlanner Pro =====\n";
            cout << "1. Event Bookings\n";
            cout << "2. Guest Management\n";
            cout << "3. Food & Drink Selection\n";
            cout << "4. Reporting & Summary\n";
            cout << "5. Log Out\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
            case 1: eventBookingMenu(guestList); break;
            case 2: {
                if (eventList.empty()) {
                    cout << "No events found. Please create an event first.\n";
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    break;
                }
                guestManagementMenu(guestList);
                break;
            }
            case 3: {
                if (eventList.empty()) {
                    cout << "No events found. Please create an event first.\n";
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    break;
                }
                foodMenu(guestList);
                break;
            }
            case 4: reportingMenu(guestList, eventList); break;
            case 5: {
                saveEventsToFile(eventList); // persist events before logout
                // Save guest data before logout
                string guestFilename = currentUser.empty() ? "guests.txt" : currentUser + "_guests.txt";
                ofstream guestFile(guestFilename);
                if (guestFile.is_open()) {
                    for (const auto& g : guestList) {
                        guestFile << g.name << "," << g.phone << "," << g.rsvp << "," << g.food << "\n";
                    }
                    guestFile.close();
                }
                cout << "Logging out...\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                shouldLogout = true;
                // Reset current user and clear in-memory data on logout
                currentUser.clear();
                guestList.clear();
                eventList.clear();
                break;
            }
            default: cout << "Invalid choice.\n"; break;
            }
        } while (!shouldLogout);
        // Loop back to authenticationMenu()
        clearScreen();
    }

    return 0;
}