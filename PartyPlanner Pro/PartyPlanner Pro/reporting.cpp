#include "reporting.h"
#include "food_selection.h"
#include <ctime>
#include <cstdlib>
#include <unordered_map>

void generateSummary(const vector<Guest>& guestList, const vector<Event>& events) {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           PARTY PLANNER PRO - SUMMARY REPORT\n";
    cout << string(50, '=') << "\n\n";

    // Event Information Section
    cout << "EVENT INFORMATION:\n";
    cout << string(30, '-') << "\n";
    if (events.empty()) {
        cout << "No events booked yet.\n\n";
    } else {
        cout << "Total Events Booked: " << events.size() << "\n";
        cout << "\nUpcoming Events:\n";
        for (size_t i = 0; i < events.size(); i++) {
            cout << (i + 1) << ". " << events[i].eventName 
                 << " - " << events[i].date << " at " << events[i].time 
                 << " (" << events[i].location << ")\n";
        }
        cout << "\n";
    }

    // Guest Information Section
    cout << "GUEST STATISTICS:\n";
    cout << string(30, '-') << "\n";
    if (guestList.empty()) {
        cout << "No guests registered yet.\n\n";
        cout << string(50, '=') << "\n";
        return;
    }

    int yesCount = 0, noCount = 0, notResponded = 0;
    int noFoodCount = 0;
    double attendanceRate = 0.0;

    for (const auto& g : guestList) {
        if (g.rsvp == "Yes" || g.rsvp == "YES") yesCount++;
        else if (g.rsvp == "No" || g.rsvp == "NO") noCount++;
        else notResponded++;

        if (g.food.empty() || g.food == "None") {
            noFoodCount++;
        }
    }

    // Calculate attendance rate
    if (guestList.size() > 0) {
        attendanceRate = (double)yesCount / guestList.size() * 100.0;
    }

    // Guest Statistics
    cout << left << setw(20) << "Total Guests:" << right << setw(10) << guestList.size() << "\n";//try
    cout << left << setw(20) << "Attending:" << right << setw(10) << yesCount << "\n";
    cout << left << setw(20) << "Not Attending:" << right << setw(10) << noCount << "\n";
    cout << left << setw(20) << "Not Responded:" << right << setw(10) << notResponded << "\n";
    cout << left << setw(20) << "Attendance Rate:" << right << setw(9) << fixed << setprecision(1) << attendanceRate << "%\n\n";

    // Food Preferences
    cout << "FOOD PREFERENCES:\n";
    cout << string(30, '-') << "\n";
    
    // Get food preferences using the proper food counting function
    vector<pair<string, int>> foodPrefs = getAllFoodPreferences(guestList);
    
    // Display food preferences from the food menu
    for (const auto& foodPair : foodPrefs) {
        cout << left << setw(20) << (foodPair.first + ":") << right << setw(10) << foodPair.second << "\n";
    }
    cout << left << setw(20) << "No Selection:" << right << setw(10) << noFoodCount << "\n\n";

    // Recommendations
    cout << "RECOMMENDATIONS:\n";
    cout << string(30, '-') << "\n";

    if (notResponded > 0) {
        cout << "- Follow up with " << notResponded << " guests who haven't responded\n";
    }

    if (noFoodCount > 0) {
        cout << "- " << noFoodCount << " guests haven't selected food preferences\n";
    }

    // Find most popular food
    int maxFood = 0;
    string mostPopular = "None";

    for (const auto& foodPair : foodPrefs) {
        if (foodPair.second > maxFood) {
            maxFood = foodPair.second;
            mostPopular = foodPair.first;
        }
    }

    if (maxFood > 0) {
        cout << "- Most popular food choice: " << mostPopular << " (" << maxFood << " guests)\n";
    }

    if (attendanceRate < 50.0) {
        cout << "- Low attendance rate - consider sending reminders\n";
    }
    else if (attendanceRate > 80.0) {
        cout << "- Excellent attendance rate!\n";
    }

    cout << "\n" << string(50, '=') << "\n";
}

void generateDetailedReport(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "\nNo guests to generate detailed report.\n";
        return;
    }

    cout << "\n" << string(60, '=') << "\n";
    cout << "              DETAILED GUEST LIST REPORT\n";
    cout << string(60, '=') << "\n\n";

    cout << left << setw(25) << "Name"
        << setw(15) << "Phone"
        << setw(15) << "RSVP Status"
        << setw(15) << "Food Choice" << "\n";
    cout << string(70, '-') << "\n";

    for (const auto& g : guestList) {
        string rsvpStatus = g.rsvp;
        if (g.rsvp == "Yes" || g.rsvp == "YES") rsvpStatus = "[YES]";
        else if (g.rsvp == "No" || g.rsvp == "NO") rsvpStatus = "[NO]";
        else rsvpStatus = g.rsvp;

        cout << left << setw(25) << g.name
            << setw(15) << g.phone
            << setw(15) << rsvpStatus
            << setw(15) << g.food << "\n";
    }

    cout << string(70, '-') << "\n";
    cout << "Total: " << guestList.size() << " guests\n";
}



void exportSummaryToCSV(const vector<Guest>& guestList, const vector<Event>& events) {
    // Generate filename with timestamp
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char filename[100];
    strftime(filename, sizeof(filename), "party_summary_%Y%m%d_%H%M%S.csv", &ltm);
    
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not create CSV file.\n";
        return;
    }
    
    // Write CSV header
    file << "Report Type,Value,Count\n";
    
    // Event information
    file << "Events,Total Events," << events.size() << "\n";
    for (size_t i = 0; i < events.size(); i++) {
        file << "Events,Event " << (i + 1) << " - " << events[i].eventName 
             << "," << events[i].date << " " << events[i].time << " (" << events[i].location << ")\n";
    }
    
    // Guest statistics
    int yesCount = 0, noCount = 0, notResponded = 0;
    int noFoodCount = 0;
    
    for (const auto& g : guestList) {
        if (g.rsvp == "Yes" || g.rsvp == "YES") yesCount++;
        else if (g.rsvp == "No" || g.rsvp == "NO") noCount++;
        else notResponded++;
        
        if (g.food.empty() || g.food == "None") {
            noFoodCount++;
        }
    }
    
    double attendanceRate = guestList.size() > 0 ? (double)yesCount / guestList.size() * 100.0 : 0.0;
    
    file << "Guests,Total Guests," << guestList.size() << "\n";
    file << "Guests,Attending," << yesCount << "\n";
    file << "Guests,Not Attending," << noCount << "\n";
    file << "Guests,Not Responded," << notResponded << "\n";
    file << "Guests,Attendance Rate %," << fixed << setprecision(1) << attendanceRate << "\n";
    
    // Food preferences using proper food counting
    vector<pair<string, int>> foodPrefs = getAllFoodPreferences(guestList);
    for (const auto& foodPair : foodPrefs) {
        file << "Food," << foodPair.first << "," << foodPair.second << "\n";
    }
    file << "Food,No Selection," << noFoodCount << "\n";
    
    file.close();
    cout << "\nSummary report exported to: " << filename << "\n";
    openFileInDefaultApp(filename);
}

void exportDetailedToCSV(const vector<Guest>& guestList) {
    if (guestList.empty()) {
        cout << "\nNo guests to export.\n";
        return;
    }
    
    // Generate filename with timestamp
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char filename[100];
    strftime(filename, sizeof(filename), "guest_list_%Y%m%d_%H%M%S.csv", &ltm);
    
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not create CSV file.\n";
        return;
    }
    
    // Write CSV header
    file << "Name,Phone,RSVP Status,Food Choice\n";
    
    // Write guest data
    for (const auto& g : guestList) {
        file << "\"" << g.name << "\",\"" << g.phone << "\",\"" << g.rsvp << "\",\"" << g.food << "\"\n";
    }
    
    file.close();
    cout << "\nDetailed guest list exported to: " << filename << "\n";
    openFileInDefaultApp(filename);
}

void openFileInDefaultApp(const string& filename) {
    cout << "Opening file in default application...\n";
    
    #ifdef _WIN32
        // Windows
        string command = "start \"\" \"" + filename + "\"";
        system(command.c_str());
    #elif __APPLE__
        // macOS
        string command = "open \"" + filename + "\"";
        system(command.c_str());
    #else
        // Linux
        string command = "xdg-open \"" + filename + "\"";
        system(command.c_str());
    #endif
    
    cout << "File should open in your default CSV viewer (Excel, etc.)\n";
}

void reportingMenu(const vector<Guest>& guestList, const vector<Event>& events) {
    int choice;
    do {
        cout << "\n--- Reporting & Summary Menu ---\n";
        cout << "1. Generate Summary Report\n";
        cout << "2. Generate Detailed Guest List\n";
        cout << "3. Export Summary to CSV\n";
        cout << "4. Export Guest List to CSV\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
        case 1: generateSummary(guestList, events); break;
        case 2: generateDetailedReport(guestList); break;
        case 3: exportSummaryToCSV(guestList, events); break;
        case 4: exportDetailedToCSV(guestList); break;
        case 0: break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);
}
