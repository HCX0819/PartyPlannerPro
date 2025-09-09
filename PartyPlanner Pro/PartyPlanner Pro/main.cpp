#include "common.h"
#include "guest_management.h"
#include "food_selection.h"
#include "reporting.h"
#include "login.h"
#include "event_booking.h"
#include "payment.h"
#include <fstream>
#include <sstream>
#include <limits> 

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    // Local variables instead of globals
    vector<Guest> guestList;
    vector<Event> eventList;
    string currentUser;
    
    // allow logout back to login screen
    while (true) {
        // Authentication returns username or empty string
        currentUser = authenticationMenu();
        if (currentUser.empty()) {
            cout << "Exiting program...\n";
            return 0;
        }
        
        // Ensure clean state before loading user-specific data
        guestList.clear();
        eventList.clear();
        
        // Load user-specific data
        loadEventsFromFile(eventList, currentUser);
        
        // Load user-specific guest data
        string guestFilename = currentUser + "_guests.txt";
        ifstream guestFile(guestFilename);
        if (guestFile.is_open()) {
            guestList.clear();
            string line;
            while (getline(guestFile, line)) {
                if (!line.empty()) {
                    // Find the positions of the first 3 commas
                    size_t pos1 = line.find(',');
                    size_t pos2 = line.find(',', pos1 + 1);
                    size_t pos3 = line.find(',', pos2 + 1);
                    
                    if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                        Guest g;
                        g.name = line.substr(0, pos1);
                        g.phone = line.substr(pos1 + 1, pos2 - pos1 - 1);
                        g.rsvp = line.substr(pos2 + 1, pos3 - pos2 - 1);
                        g.food = line.substr(pos3 + 1); // Everything after the 3rd comma is food
                        guestList.push_back(g);
                    }
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
            cout << "5. Payments\n";
            cout << "6. Log Out\n";
            cout << "Enter choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            switch (choice) {
            case 1: eventBookingMenu(guestList, eventList); break;
            case 2: {
                if (eventList.empty()) {
                    cout << "No events found. Please create an event first.\n";
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    break;
                }
                guestManagementMenu(guestList, currentUser);
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
                foodMenu(guestList, currentUser);
                break;
            }
            case 4: reportingMenu(guestList, eventList); break;
            case 5: {
                // Clear leftover newline before switching to getline-based input in payments
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                paymentMenu(eventList, guestList, currentUser);
                break;
            }
            case 6: {
                saveEventsToFile(eventList, currentUser); // persist events before logout
                // Save guest data before logout
                string guestFilename = currentUser + "_guests.txt";
                ofstream guestFile(guestFilename);
                if (guestFile.is_open()) {
                    for (const auto& g : guestList) {
                        guestFile << g.name << "," << g.phone << "," << g.rsvp << "," << g.food << "\n";
                    }
                    guestFile.close();
                    cout << "Guest data saved successfully.\n";
                } else {
                    cout << "Warning: Could not save guest data to " << guestFilename << "\n";
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