#include "guest_management.h"

void registerGuest(vector<Guest>& guestList) {
    Guest g;
    cout << "Enter guest name: ";
    cin.ignore();
    getline(cin, g.name);

    cout << "Enter phone number (10-11 digits): ";
    cin >> g.phone;
    while (g.phone.length() < 10 || g.phone.length() > 11) {
        cout << "Invalid. Enter again: ";
        cin >> g.phone;
    }

    g.rsvp = "Not Responded";
    g.food = "None";
    guestList.push_back(g);
    cout << "Guest registered!\n";
}

void updateRSVP(vector<Guest>& guestList) {
    string name, reply;
    cout << "Enter guest name to update RSVP: ";
    cin.ignore();
    getline(cin, name);

    for (auto& g : guestList) {
        if (g.name == name) {
            cout << "RSVP (Yes/No): ";
            cin >> reply;
            while (reply != "Yes" && reply != "No") {
                cout << "Invalid. Enter Yes/No: ";
                cin >> reply;
            }
            g.rsvp = reply;
            cout << "RSVP updated!\n";
            return;
        }
    }
    cout << "Guest not found.\n";
}

void viewGuestList(const vector<Guest>& guestList) {
    cout << left << setw(20) << "Name"
        << setw(15) << "Phone"
        << setw(15) << "RSVP"
        << setw(15) << "Food" << "\n";
    cout << "---------------------------------------------------\n";
    for (const auto& g : guestList) {
        cout << left << setw(20) << g.name
            << setw(15) << g.phone
            << setw(15) << g.rsvp
            << setw(15) << g.food << "\n";
    }
}

void guestManagementMenu(vector<Guest>& guestList) {
    int choice;
    do {
        cout << "\n--- Guest Management ---\n";
        cout << "1. Register Guest\n";
        cout << "2. Update RSVP\n";
        cout << "3. View Guest List\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: registerGuest(guestList); break;
        case 2: updateRSVP(guestList); break;
        case 3: viewGuestList(guestList); break;
        case 4: break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 4);
}
