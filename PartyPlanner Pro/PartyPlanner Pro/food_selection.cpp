#include "food_selection.h"
#include <limits>
#include <fstream>
#include <iomanip>

const double COST_PER_PERSON = 20.0; // fixed cost per guest

// Shared food items list (used by both food selection and reporting)
const vector<string> FOOD_ITEMS = {
    "Pasta", "Nasi lemak", "Pizza", "Burger", "Fried Chicken",
    "French fries", "Cake", "Soda drink", "Iced lemon tea", "Tea", "Mineral Water"
};

// Helper: food menu
string getFoodName(int choice) {
    switch (choice) {
    case 1: return "Pasta";
    case 2: return "Nasi lemak";
    case 3: return "Pizza";
    case 4: return "Burger";
    case 5: return "Fried Chicken";
    case 6: return "French fries";
    case 7: return "Cake";
    case 8: return "Soda drink";
    case 9: return "Iced lemon tea";
    case 10: return "Tea";
    case 11: return "Mineral Water";
    default: return "";
    }
}

// Helper: Count food selections
int countFoodSelections(const string& foodString) {
    if (foodString.empty() || foodString == "None") {
        return 0;
    }
    // Count commas + 1 (since each food is separated by comma)
    int count = 1;
    for (char c : foodString) {
        if (c == ',') count++;
    }
    return count;
}

// Helper: Check if food already exists (simple way)
bool foodAlreadyExists(const string& foodList, const string& newFood) {
    if (foodList.empty() || foodList == "None") {
        return false;
    }

    // Simple check: if the exact food name is in the list
    // We look for ", newFood" or "newFood," or just "newFood" (if it's the only item)
    string searchPattern1 = ", " + newFood;
    string searchPattern2 = newFood + ",";

    return (foodList.find(searchPattern1) != string::npos) ||
        (foodList.find(searchPattern2) != string::npos) ||
        (foodList == newFood);
}

// Helper: Make string lowercase (simple way)
string toLowerCase(const string& str) {
    string result = str;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c = c + 32; // Convert to lowercase
        }
    }
    return result;
}

// Helper: Check if guest names match (case-insensitive)
bool guestNameMatches(const string& guestName, const string& inputName) {
    return toLowerCase(guestName) == toLowerCase(inputName);
}


// Helper: Count how many times a specific food item is selected
int countFoodPreference(const vector<Guest>& guestList, const string& foodItem) {
    int count = 0;
    for (const auto& guest : guestList) {
        if (!guest.food.empty() && guest.food != "None") {
            // Check if the food item appears in the guest's food list
            if (foodAlreadyExists(guest.food, foodItem)) {
                count++;
            }
        }
    }
    return count;
}

// Helper: Get all food preferences with counts (for reporting)
vector<pair<string, int>> getAllFoodPreferences(const vector<Guest>& guestList) {
    vector<pair<string, int>> foodPrefs;

    for (const string& food : FOOD_ITEMS) {
        int count = countFoodPreference(guestList, food);
        if (count > 0) {
            foodPrefs.push_back(make_pair(food, count));
        }
    }

    return foodPrefs;
}


// Helper: Display food menu
void displayFoodMenu() {
    cout << "\n" << string(45, '=') << "\n";
    cout << "              PPP FOOD MENU\n";
    cout << string(45, '=') << "\n";
    cout << "\nSelect food (0 to finish):\n\n";
    cout << " 1. Pasta\n";
    cout << " 2. Nasi lemak\n";
    cout << " 3. Pizza\n";
    cout << " 4. Burger\n";
    cout << " 5. Fried Chicken\n";
    cout << " 6. French fries\n";
    cout << " 7. Cake\n";
    cout << " 8. Soda drink\n";
    cout << " 9. Iced lemon tea\n";
    cout << " 10. Tea\n";
    cout << " 11. Mineral Water\n";
    cout << "\n" << string(45, '-') << "\n";
}

// Helper: Save guest data to file
bool saveGuestData(const vector<Guest>& guestList) {
    extern string currentUser;
    string filename = currentUser.empty() ? "guests.txt" : currentUser + "_guests.txt";
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not save guest data to " << filename << "\n";
        return false;
    }

    for (const auto& guest : guestList) {
        file << guest.name << "," << guest.phone << "," << guest.rsvp << "," << guest.food << "\n";
    }
    file.close();
    return true;
}

// Allow multiple food selections
void chooseFood(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    // Display registered guests with numbers
    cout << "\n" << string(60, '=') << "\n";
    cout << "                REGISTERED GUESTS\n";
    cout << string(60, '=') << "\n";
    cout << left << setw(5) << "No." << setw(25) << "Guest Name" << setw(15) << "RSVP Status" << "Phone\n";
    cout << string(60, '-') << "\n";
    
    for (int i = 0; i < guestList.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(25) << guestList[i].name 
             << setw(15) << guestList[i].rsvp << guestList[i].phone << "\n";
    }
    cout << string(60, '=') << "\n";

    int guestChoice;
    while (true) {
        cout << "\nEnter guest number to select food (0 to cancel): ";
        cin >> guestChoice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (guestChoice < 0 || guestChoice > guestList.size()) {
            cout << "Invalid choice. Please select 1-" << guestList.size() << " or 0 to cancel.\n";
            continue;
        }
        break;
    }

    if (guestChoice == 0) {
        cout << "Food selection cancelled.\n";
        return;
    }

    Guest& g = guestList[guestChoice - 1];
    int choice;
    do {
        int currentCount = countFoodSelections(g.food);

        cout << "\nMaximum: 5 selections\n";
        cout << "\nGuest: " << g.name << "\n";
        cout << "Current choices (" << currentCount << "/5): "
            << (g.food.empty() || g.food == "None" ? "None" : g.food) << "\n";

        displayFoodMenu();
        cout << "\nEnter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            cout << "\nFinished selecting food.\n";
            break;
        }

        string newFood = getFoodName(choice);
        if (newFood.empty()) {
            cout << "Invalid choice. Please select 1-11 or 0 to finish.\n";
            continue;
        }

        // Check if already at maximum
        if (currentCount >= 5) {
            cout << "Error: Maximum 5 food selections allowed. You have already selected 5 items.\n";
            continue;
        }

        // Prevent duplicates
        if (!foodAlreadyExists(g.food, newFood)) {
            if (!g.food.empty() && g.food != "None") {
                g.food += ", " + newFood;
            }
            else {
                g.food = newFood;
            }
            cout << "\nAdded " << newFood << " for " << g.name << ".\n";
        }
        else {
            cout << "\n" << g.name << " already selected " << newFood << ".\n";
        }
    } while (choice != 0);

    // Save changes
    if (saveGuestData(guestList)) {
        cout << "\nFood selections saved successfully.\n";
    }
}

void editGuestFood(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    // Display registered guests with numbers
    cout << "\n" << string(60, '=') << "\n";
    cout << "                REGISTERED GUESTS\n";
    cout << string(60, '=') << "\n";
    cout << left << setw(5) << "No." << setw(25) << "Guest Name" << setw(15) << "RSVP Status" << "Phone\n";
    cout << string(60, '-') << "\n";
    
    for (int i = 0; i < guestList.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(25) << guestList[i].name 
             << setw(15) << guestList[i].rsvp << guestList[i].phone << "\n";
    }
    cout << string(60, '=') << "\n";

    int guestChoice;
    while (true) {
        cout << "\nEnter guest number to edit food (0 to cancel): ";
        cin >> guestChoice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (guestChoice < 0 || guestChoice > guestList.size()) {
            cout << "Invalid choice. Please select 1-" << guestList.size() << " or 0 to cancel.\n";
            continue;
        }
        break;
    }

    if (guestChoice == 0) {
        cout << "Food editing cancelled.\n";
        return;
    }

    Guest& g = guestList[guestChoice - 1];
    cout << "\nCurrent food choices for " << g.name << ": "
        << (g.food.empty() || g.food == "None" ? "None" : g.food) << "\n\n";
    cout << "Do you want to clear and reselect? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    if (confirm == 'y' || confirm == 'Y') {
        g.food = "None";  // reset
        int choice;
        do {
            int currentCount = countFoodSelections(g.food);

            cout << "\nMaximum: 5 selections\n";
            cout << "\nGuest: " << g.name << "\n";
            cout << "Current choices (" << currentCount << "/5): "
                << (g.food.empty() || g.food == "None" ? "None" : g.food) << "\n";

            displayFoodMenu();
            cout << "\nEnter choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 0) {
                cout << "\nFinished editing food selections.\n";
                break;
            }

            string newFood = getFoodName(choice);
            if (newFood.empty()) {
                cout << "Invalid choice. Please select 1-11 or 0 to finish.\n";
                continue;
            }

            // Check if already at maximum
            if (currentCount >= 5) {
                cout << "Error: Maximum 5 food selections allowed. You have already selected 5 items.\n";
                continue;
            }

            // Prevent duplicates
            if (!foodAlreadyExists(g.food, newFood)) {
                if (!g.food.empty() && g.food != "None") {
                    g.food += ", " + newFood;
                }
                else {
                    g.food = newFood;
                }
                cout << "\nAdded " << newFood << " for " << g.name << ".\n";
            }
            else {
                cout << "\n" << g.name << " already selected " << newFood << ".\n";
            }
        } while (choice != 0);

        // Save changes
        if (saveGuestData(guestList)) {
            cout << "\nFood selections updated and saved successfully.\n";
        }
    }
    else {
        cout << "Edit cancelled.\n";
    }
}

void viewGuestChoices(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    cout << "\n" << string(60, '=') << "\n";
    cout << "                GUEST FOOD SELECTIONS\n";
    cout << string(60, '=') << "\n\n";
    cout << left << setw(25) << "Guest Name" << setw(15) << "RSVP Status" << "Food Choices\n";
    cout << string(60, '-') << "\n";

    for (const auto& g : guestList) {
        cout << left << setw(25) << g.name << setw(15) << g.rsvp
            << (g.food.empty() || g.food == "None" ? "No food selected" : g.food) << "\n";
    }
    cout << "\n" << string(60, '=') << "\n";
}


void foodMenu(vector<Guest>& guestList) {
    int choice;
    do {
        cout << "\n" << string(45, '=') << "\n";
        cout << "           FOOD MANAGEMENT MENU\n";
        cout << string(45, '=') << "\n\n";
        cout << " 1. Choose Food for Guest\n";
        cout << " 2. Edit Guest Food Selection\n";
        cout << " 3. View All Food Choices\n";
        cout << " 0. Return to Main Menu\n\n";
        cout << string(45, '-') << "\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            chooseFood(guestList);
            break;
        case 2:
            editGuestFood(guestList);
            break;
        case 3:
            viewGuestChoices(guestList);
            break;
        case 0:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice. Please select 0-3.\n";
            break;
        }
    } while (choice != 0);
}