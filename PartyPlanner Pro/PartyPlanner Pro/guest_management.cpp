#include "guest_management.h"
#include <sstream>

using namespace std;


void registerGuest(vector<Guest>& guestList) {
    Guest g;
    cout << "Enter guest name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, g.name);

    regex phonePattern(R"(^\+60\d{9,10}$)");
    // ^ : start of string
    // \+60 : must start with +60
    // \d{9,10} : must have 9 or 10 digits after +60
    // $ : end of string

    cout << "Enter phone number (must start with +60): ";
    cin >> g.phone;

    while (!regex_match(g.phone, phonePattern)) {
        cout << "Invalid. Enter again (format: +60XXXXXXXXX): ";
        cin >> g.phone;
    }

    g.rsvp = "Not Responded";
    guestList.push_back(g);

    cout << "\nGuest registered successfully!\n";
}

void editGuestDetail(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests registered yet.\n";
        return;
    }

    // Show guest list
    cout << "\n============= Guest List =============\n";
    for (int i = 0; i < guestList.size(); i++) {
        cout << i + 1 << ". " << guestList[i].name
            << " (" << guestList[i].phone << ") - RSVP: "
            << guestList[i].rsvp << "\n";
    }

    int choice;
    while (true) {
        cout << "Enter the number of the guest to edit (0 to cancel): ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue; // ask again
        }

        if (choice < 0 || choice > guestList.size()) {
            cout << "Invalid choice. Try again.\n";
            continue; // ask again
        }
        break; // input is valid, exit loop
    }

    if (choice == 0) {
        cout << "Edit cancelled.\n";
        return;
    }

    Guest& g = guestList[choice - 1];

    int editOption;
    while (true) {
        cout << "\nWhat do you want to edit?\n";
        cout << "1. Name\n";
        cout << "2. Phone Number\n";
        cout << "Enter choice: ";
        cin >> editOption;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue; // retry
        }

        if (editOption == 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover
            cout << "Enter new name: ";
            getline(cin, g.name);
            cout << "Name updated successfully!\n";
            break;
        }
        else if (editOption == 2) {
            string newPhone;
            cout << "Enter new phone number (start with +60): ";
            cin >> newPhone;
            if (newPhone.rfind("+60", 0) == 0 && newPhone.length() >= 11 && newPhone.length() <= 13) {
                g.phone = newPhone;
                cout << "Phone number updated successfully!\n";
            }
            else {
                cout << "Invalid phone number format.\n";
            }
            break;
        }
        else {
            cout << "Invalid option. Try again.\n";
        }
    }
}



void deleteGuest(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests registered yet.\n";
        return;
    }

    cout << "\n============= Guest List =============\n";
    for (int i = 0; i < guestList.size(); i++) {
        cout << i + 1 << ". " << guestList[i].name
            << " (" << guestList[i].phone << ") - RSVP: "
            << guestList[i].rsvp << "\n";
    }

    int choice;
    while (true) {
        cout << "Enter the number of the guest to delete (0 to cancel): ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue; // ask again
        }

        if (choice < 0 || choice > guestList.size()) {
            cout << "Invalid choice. Try again.\n";
            continue; // ask again
        }

        break; // valid input
    }

    if (choice == 0) {
        cout << "Delete cancelled.\n";
        return;
    }

    cout << "Guest \"" << guestList[choice - 1].name << "\" deleted successfully!\n";
    guestList.erase(guestList.begin() + (choice - 1));
}




void updateRSVP(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests registered yet.\n";
        return;
    }

    cout << "\n============= Guest List =============\n";
    for (int i = 0; i < guestList.size(); i++) {
        cout << i + 1 << ". " << guestList[i].name
            << " (" << guestList[i].phone << ") - RSVP: "
            << guestList[i].rsvp << "\n";
    }

    int choice;
    while (true) {
        cout << "Enter the number of the guest to update RSVP (0 to go back): ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue; // ask again
        }

        if (choice < 0 || choice > guestList.size()) {
            cout << "Invalid choice. Try again.\n";
            continue; // ask again
        }

        break; // valid input
    }

    if (choice == 0) {
        cout << "Returning to previous menu...\n";
        return;
    }

    string reply;
    while (true) {
        cout << "RSVP (Yes/No): ";
        cin >> reply;

        // Convert to uppercase
        for (auto& c : reply) c = toupper(c);

        if (reply == "YES" || reply == "NO") {
            guestList[choice - 1].rsvp = reply;  // stored as YES or NO
            cout << "RSVP updated!\n";
            break;
        }
        else {
            cout << "Invalid input. Please type Yes or No.\n";
        }
    }
}


void searchGuest(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests registered yet.\n";
        return;
    }

    string pattern;
    cout << "Enter keyword (name or phone pattern): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, pattern);

    // Create regex (case-insensitive)
    regex re(pattern, regex_constants::icase);

    bool found = false;
    for (const auto& g : guestList) {
        if (regex_search(g.name, re) || regex_search(g.phone, re)) {
            cout << "\nGuest Found:\n";
            cout << "Name: " << g.name << "\n";
            cout << "Phone: " << g.phone << "\n";
            cout << "RSVP: " << g.rsvp << "\n";
            found = true;
        }
    }

    if (!found) cout << "No guest matched the search.\n";
}

void viewGuestList(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "\nNo guests registered yet.\n";
        return;
    }

    cout << "\n============= Guest List =============\n";
    cout << left << setw(20) << "Name"
        << setw(15) << "Phone"
        << setw(15) << "RSVP" << "\n";
    cout << "---------------------------------------\n";
    for (const auto& g : guestList) {
        cout << left << setw(20) << g.name
            << setw(15) << g.phone
            << setw(15) << g.rsvp << "\n";
    }
    cout << "=====================================\n";
}

void showRSVPStats(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests registered yet.\n";
        return;
    }

    const int rows = 3; // Yes, No, Not Responded
    const int cols = 2; // 0 = count, 1 = percentage
    double stats[rows][cols] = { 0 };

    int total = guestList.size();

    // Count RSVP
    for (const auto& g : guestList) {
        if (g.rsvp == "YES") stats[0][0]++;
        else if (g.rsvp == "NO") stats[1][0]++;
        else stats[2][0]++; // Not Responded
    }

    // Calculate percentage
    for (int i = 0; i < rows; i++) {
        stats[i][1] = (stats[i][0] / static_cast<double>(total)) * 100.0;
    }

    string labels[rows] = { "Yes", "No", "Not Responded" };

    // Print table header
    cout << "\n================ RSVP Statistics ===============\n";
    cout << left << setw(20) << "Status"
        << setw(10) << "Count"
        << setw(10) << "Percent\n";
    cout << "----------------------------------------------\n";

    // Print rows
    for (int i = 0; i < rows; i++) {
        cout << left << setw(20) << labels[i]
            << setw(10) << static_cast<int>(stats[i][0])
            << setw(9) << fixed << setprecision(1) << stats[i][1] << "%\n";
    }

    cout << "================================================\n";
    cout << "Total Guests: " << total << "\n";
    cout << "================================================\n";
}




void saveGuestsToFile(const vector<Guest>& guestList, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& guest : guestList) {
            file << guest.name << ","
                << guest.phone << ","
                << guest.rsvp << ","
                << guest.food << "\n";
        }
        file.close();
        cout << "Guest data saved successfully to " << filename << "!\n";
    }
    else {
        cout << "Error: Unable to save guest data to " << filename << "\n";
    }
}

void loadGuestsFromFile(vector<Guest>& guestList, const string& filename) {
    ifstream file(filename);
    if (!file) {
        // Ensure no stale data remains when the user has no file yet
        guestList.clear();
        cout << "Error: Cannot open file '" << filename << "'\n";
        cout << "Please make sure you've saved data first using option 7.\n";
        return;
    }

    guestList.clear();
    string line;
    int loadedCount = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Guest guest;
        string name, phone, rsvp, food;
        bool okName = static_cast<bool>(getline(ss, name, ','));
        bool okPhone = static_cast<bool>(getline(ss, phone, ','));
        bool okRsvp = static_cast<bool>(getline(ss, rsvp, ','));
        bool okFood = static_cast<bool>(getline(ss, food, ','));

        if (okName && okPhone && okRsvp) {
            guest.name = name;
            guest.phone = phone;
            guest.rsvp = rsvp;
            guest.food = okFood ? food : string(""); // tolerate legacy 3-field rows
            guestList.push_back(guest);
            loadedCount++;
        }
    }

    file.close();

    if (loadedCount == 0) {
        cout << "No guests loaded from " << filename << ". The file may be empty or incorrectly formatted.\n";
    }
    else {
        cout << "Successfully loaded " << loadedCount << " guests from " << filename << "!\n";
    }

    // Open the file in Notepad only after loading
    system(("notepad " + filename).c_str());
}


void guestManagementMenu(vector<Guest>& guestList) {
    extern string currentUser;
    string filename = currentUser + "_guests.txt";
    loadGuestsFromFile(guestList, filename);

    int choice;
    do {
        cout << "\n====== Guest Management ======\n";
        cout << "1. Register Guest\n";
        cout << "2. Edit Guest\n";
        cout << "3. Delete Guest\n";
        cout << "4. Update RSVP\n";
        cout << "5. View Guest List\n";
        cout << "6. Show RSVP Stats\n";
        cout << "7. Save to File\n";
        cout << "8. Load from File\n";
        cout << "9. Search Guest\n";
        cout << "10. Return to Main Menu\n";
        cout << "==============================\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: registerGuest(guestList); saveGuestsToFile(guestList, filename); break;
        case 2: editGuestDetail(guestList); saveGuestsToFile(guestList, filename); break;
        case 3: deleteGuest(guestList); saveGuestsToFile(guestList, filename); break;
        case 4: updateRSVP(guestList); saveGuestsToFile(guestList, filename); break;
        case 5: viewGuestList(guestList); break;
        case 6: showRSVPStats(guestList); break;
        case 7: {
            saveGuestsToFile(guestList, filename);
            break;
        }
        case 8: {
            loadGuestsFromFile(guestList, filename);
            break;
        }
        case 9: searchGuest(guestList); break;   // <-- Added
        case 10: cout << "Returning to main menu...\n"; break;
        default: cout << "Invalid choice. Please try again.\n"; break;
        }
    } while (choice != 10);
}
