#include "food_selection.h"

void chooseFood(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    string name;
    cout << "Enter guest name to select food: ";
    cin.ignore();
    getline(cin, name);

    for (auto& g : guestList) {
        if (g.name == name) {
            int choice;
            cout << "Select food:\n1. Pizza\n2. Cake\n3. Drinks\n4. Sandwich\n5. Salad\n";
            cin >> choice;

            switch (choice) {
            case 1: g.food = "Pizza"; break;
            case 2: g.food = "Cake"; break;
            case 3: g.food = "Drinks"; break;
            case 4: g.food = "Sandwich"; break;
            case 5: g.food = "Salad"; break;
            default: cout << "Invalid choice.\n"; return;
            }
            cout << "Food updated for " << g.name << "!\n";
            // Autosave after update
            extern string currentUser;
            string filename = currentUser.empty() ? "guests.txt" : currentUser + "_guests.txt";
            ofstream file(filename);
            if (file.is_open()) {
                for (const auto& guest : guestList) {
                    file << guest.name << "," << guest.phone << "," << guest.rsvp << "," << guest.food << "\n";
                }
                file.close();
            }
            return;
        }
    }
    cout << "Guest not found.\n";
}