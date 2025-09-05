#ifndef LOGIN_H
#define LOGIN_H

#include "common.h"

struct UserAccount {
    string username;
    string salt;
    string passwordHash;
};

// Top-level auth menu. Returns true if a user logged in successfully.
bool authenticationMenu();

// Utility used internally but exposed for potential reuse/tests
string hashPassword(const string& salt, const string& password);

#endif


