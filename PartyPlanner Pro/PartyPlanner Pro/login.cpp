#include "login.h"
#include <ctime>
#include <cstdlib>
#include <limits>

static const char* kUserDbFile = "users.db";

static void clearScreenAuth() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void centerPrint(const string& text, size_t width) {
    size_t len = text.size();
    if (len >= width) { cout << text << "\n"; return; }
    size_t pad = (width - len) / 2;
    cout << string(pad, ' ') << text << "\n";
}

static void printAuthHeader() {
    clearScreenAuth();
    const size_t width = 80;
    const string border = ".~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.~~.";
    cout << border.substr(0, width) << "\n\n";

    cout << R"( 
 ____                __                                
/\  _`\             /\ \__                             
\ \ \L\ \ __    _ __\ \ ,_\ __  __                     
 \ \ ,__/'__`\ /\`'__\ \ \//\ \/\ \                    
  \ \ \/\ \L\.\\ \ \/ \ \ \\ \ \_\ \                   
   \ \_\ \__/.\_\ \_\  \ \__\/`____ \                  
    \/_/\/__/\/_/\/_/   \/__/`/___/> \                 
                                /\___/                 
                                \/__/                  
 ____    ___                                           
/\  _`\ /\_ \                                          
\ \ \L\ \//\ \      __      ___     ___      __  _ __  
 \ \ ,__/ \ \ \   /'__`\  /' _ `\ /' _ `\  /'__`/\`'__\
  \ \ \/   \_\ \_/\ \L\.\_/\ \/\ \/\ \/\ \/\  __\ \ \/ 
   \ \_\   /\____\ \__/.\_\ \_\ \_\ \_\ \_\ \____\ \_\ 
    \/_/   \/____/\/__/\/_/\/_/\/_/\/_/\/_/\/____/\/_/ 
                                                       
                                                       
 ____                                                  
/\  _`\                                                
\ \ \L\ \_ __  ___                                     
 \ \ ,__/\`'__/ __`\                                   
  \ \ \/\ \ \/\ \L\ \                                  
   \ \_\ \ \_\ \____/                                  
    \/_/  \/_/\/___/                                   
)" << "\n";

    cout << "\n";
    cout << border.substr(0, width) << "\n\n";
}




static string generateSalt() {
    static const char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string salt;
    srand((unsigned)time(nullptr));
    for (int i = 0; i < 8; ++i) {
        salt.push_back(alphabet[rand() % 62]);
    }
    return salt;
}

string hashPassword(const string& salt, const string& password) {
    // Simple Fowler–Noll–Vo hash variant on salt+password
    const string input = salt + ":" + password;
    const unsigned int FNV_PRIME = 16777619u;
    unsigned int hash = 2166136261u;
    for (unsigned char c : input) {
        hash ^= c;
        hash *= FNV_PRIME;
    }
    // Return as hex
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

static vector<UserAccount> loadUsers() {
    vector<UserAccount> users;
    ifstream in(kUserDbFile);
    if (!in.is_open()) return users;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string username, salt, hash;
        if (getline(ss, username, ',') && getline(ss, salt, ',') && getline(ss, hash, ',')) {
            users.push_back({ username, salt, hash });
        }
    }
    return users;
}

static void saveUser(const UserAccount& user) {
    ofstream out(kUserDbFile, ios::app);
    out << user.username << "," << user.salt << "," << user.passwordHash << "\n";
}

static bool usernameExists(const vector<UserAccount>& users, const string& username) {
    return any_of(users.begin(), users.end(), [&](const UserAccount& u){ return u.username == username; });
}

static bool handleSignUp() {
    printAuthHeader();
    cout << "--- Create your account ---\n";
    cout << "Enter username: ";
    string username; cin >> username;
    cout << "Enter password (min 6 chars): ";
    string password; cin >> password;
    if (password.size() < 6) {
        cout << "Password too short. Please use at least 6 characters.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return false;
    }

    vector<UserAccount> users = loadUsers();
    if (usernameExists(users, username)) {
        cout << "Username already exists.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return false;
    }

    string salt = generateSalt();
    string hashed = hashPassword(salt, password);
    saveUser({ username, salt, hashed });
    cout << "Account created. You can now log in.\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    return true;
}

static bool handleLogIn() {
    printAuthHeader();
    cout << "--- Log in ---\n";
    cout << "Enter username: ";
    string username; cin >> username;
    cout << "Enter password: ";
    string password; cin >> password;

    vector<UserAccount> users = loadUsers();
    for (const auto& u : users) {
        if (u.username == username) {
            if (hashPassword(u.salt, password) == u.passwordHash) {
                cout << "Login successful. Welcome, " << username << "!\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                // Set current user for data isolation
                extern string currentUser;
                currentUser = username;
                return true;
            } else {
                cout << "Invalid password.\n";
                cout << "Press Enter to try again...";
                cin.ignore();
                cin.get();
                return false;
            }
        }
    }
    cout << "User not found.\n";
    cout << "Press Enter to try again...";
    cin.ignore();
    cin.get();
    return false;
}

bool authenticationMenu() {
    int choice = 0;
    do {
        printAuthHeader();
        cout << "1) Log In\n";
        cout << "2) Sign Up\n";
        cout << "3) Exit\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        switch (choice) {
            case 1: if (handleLogIn()) return true; break;
            case 2: handleSignUp(); break;
            case 3: return false;
            default: cout << "Invalid choice.\n"; break;
        }
    } while (true);
}

