#include "common.h"
#include "guest_management.h"
#include "food_selection.h"
#include "reporting.h"
#include "event_booking.h"

vector<Guest> guestList;  // define global variable

int main() {
    int choice;
    do {
        cout << "\n===== PartyPlanner Pro =====\n";
        cout << "1. Guest Management\n";
        cout << "2. Food & Drink Selection\n";
        cout << "3. Reporting & Summary\n";
        cout << "4. Event Bookings\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: guestManagementMenu(guestList); break;
        case 2: chooseFood(guestList); break;
        case 3: reportingMenu(guestList); break;
        case 4: eventBookingMenu(guestList); break;
        case 5: cout << "Exiting program...\n"; break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 5);

    return 0;
}