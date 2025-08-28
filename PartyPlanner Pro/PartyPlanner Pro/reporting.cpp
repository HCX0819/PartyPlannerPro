#include "reporting.h"

void generateSummary(const vector<Guest>& guestList) {
    int yesCount = 0, noCount = 0, notResponded = 0;
    int foodCount[5] = { 0 };

    for (const auto& g : guestList) {
        if (g.rsvp == "Yes") yesCount++;
        else if (g.rsvp == "No") noCount++;
        else notResponded++;

        if (g.food == "Pizza") foodCount[0]++;
        else if (g.food == "Cake") foodCount[1]++;
        else if (g.food == "Drinks") foodCount[2]++;
        else if (g.food == "Sandwich") foodCount[3]++;
        else if (g.food == "Salad") foodCount[4]++;
    }

    cout << "\n--- Party Summary ---\n";
    cout << "Total Guests: " << guestList.size() << "\n";
    cout << "Attending: " << yesCount << "\n";
    cout << "Not Attending: " << noCount << "\n";
    cout << "Not Responded: " << notResponded << "\n";

    cout << "\nFood Preferences:\n";
    cout << "Pizza: " << foodCount[0] << "\n";
    cout << "Cake: " << foodCount[1] << "\n";
    cout << "Drinks: " << foodCount[2] << "\n";
    cout << "Sandwich: " << foodCount[3] << "\n";
    cout << "Salad: " << foodCount[4] << "\n";
}