#include "food_selection.h"
#include <limits>
#include <fstream>

const double COST_PER_PERSON = 20.0; // fixed cost per guest

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

// Allow multiple food selections
void chooseFood(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    string name;
    cout << "Enter guest name to select food: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    for (auto& g : guestList) {
        if (g.name == name) {
            int choice;
            do {
                cout << "\n" << string(40, '=') << "\n";
                cout << "          FOOD SELECTION MENU\n";
                cout << string(40, '=') << "\n";
                cout << "Guest: " << g.name << "\n";
                cout << "Current choices: "
                    << (g.food.empty() || g.food == "None" ? "None" : g.food) << "\n";
                cout << string(40, '-') << "\n";

                cout << "Select food (0 to finish):\n";
                cout << "  1.  Pasta\n";
                cout << "  2.  Nasi lemak\n";
                cout << "  3.  Pizza\n";
                cout << "  4.  Burger\n";
                cout << "  5.  Fried Chicken\n";
                cout << "  6.  French fries\n";
                cout << "  7.  Cake\n";
                cout << "  8.  Soda drink\n";
                cout << "  9.  Iced lemon tea\n";
                cout << "  10. Tea\n";
                cout << "  11. Mineral Water\n";
                cout << string(40, '-') << "\n";
                cout << "Enter choice: ";

                cin >> choice;

                string newFood;
                switch (choice) {
                case 0:
                    cout << "Finished selecting food.\n";
                    break;
                case 1: newFood = "Pasta"; break;
                case 2: newFood = "Nasi lemak"; break;
                case 3: newFood = "Pizza"; break;
                case 4: newFood = "Burger"; break;
                case 5: newFood = "Fried Chicken"; break;
                case 6: newFood = "French fries"; break;
                case 7: newFood = "Cake"; break;
                case 8: newFood = "Soda drink"; break;
                case 9: newFood = "Iced lemon tea"; break;
                case 10: newFood = "Tea"; break;
                case 11: newFood = "Mineral Water"; break;
                default:
                    cout << "Invalid choice.\n";
                    continue;
                }

                if (choice != 0) {
                    // Prevent duplicates
                    if (g.food.find(newFood) == string::npos) {
                        if (!g.food.empty() && g.food != "None") {
                            g.food += ", " + newFood;
                        }
                        else {
                            g.food = newFood;
                        }
                        cout << " Added " << newFood << " for " << g.name << ".\n";
                    }
                    else {
                        cout << g.name << " already selected " << newFood << ".\n";
                    }
                }
            } while (choice != 0);

            return;
        }
    }
    cout << "Guest not found.\n";
}

void editGuestFood(vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    string name;
    cout << "Enter guest name to edit food: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    for (auto& g : guestList) {
        if (g.name == name) {
            cout << "\nCurrent food choices for " << g.name << ": "
                << (g.food.empty() ? "None" : g.food) << "\n";

            cout << "Do you want to clear and reselect? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                g.food = "None";  // reset
                int choice;
                do {
                    cout << "\nSelect food (0 to finish):\n";
                    cout << "1. Pasta\n2. Nasi lemak\n3. Pizza\n4. Burger\n5. Fried Chicken\n6. French fries\n7. Cake\n8. Soda drink\n9. Iced lemon tea\n10. Tea\n11. Mineral Water\n";
                    cout << "Enter choice: ";
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

void viewGuestChoices(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "No guests available.\n";
        return;
    }

    cout << "\n" << string(40, '=') << "\n";
    cout << "   GUEST FOOD SELECTIONS\n";
    cout << string(40, '=') << "\n";

    for (const auto& g : guestList) {
        cout << g.name << " (" << g.rsvp << "): "
            << (g.food.empty() ? "No food selected" : g.food) << "\n";
    }
    cout << string(40, '=') << "\n";
}

// Calculate total food budget
void calculateFoodBudget(const vector<Guest>& guestList) {
    int attendingCount = 0;
    for (const auto& g : guestList) {
        if (g.rsvp == "Yes") attendingCount++;
    }

    double totalCost = attendingCount * COST_PER_PERSON;

    cout << "\n" << string(40, '=') << "\n";
    cout << "   EVENT FOOD BUDGET (Per Person Basis)\n";
    cout << string(40, '=') << "\n";
    cout << "Guests Attending: " << attendingCount << "\n";
    cout << "Cost per Person: RM" << fixed << setprecision(2) << COST_PER_PERSON << "\n";
    cout << "----------------------------------------\n";
    cout << "Total Food Budget: RM" << fixed << setprecision(2) << totalCost << "\n";
    cout << string(40, '=') << "\n";
}

void foodMenu(vector<Guest>& guestList) {
    int choice;
    do {
        cout << "\n====== Food Management ======\n";
        cout << "1. Choose Food\n";
        cout << "2. Edit Guest Food\n";
        cout << "3. View Food Choices\n";
        cout << "4. Calculate Food Budget\n";
        cout << "5. Return to Main Menu\n";
        cout << "=============================\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: chooseFood(guestList); break;
        case 2: editGuestFood(guestList); break;
        case 3: viewGuestChoices(guestList); break;
        case 4: calculateFoodBudget(guestList); break;
        case 5: cout << "Returning...\n"; break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 5);
}