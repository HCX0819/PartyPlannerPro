#ifndef GUEST_MANAGEMENT_H
#define GUEST_MANAGEMENT_H
#include "common.h"

void guestManagementMenu(vector<Guest>& guestList, const string& currentUser);
void saveGuestsToFile(const vector<Guest>& guestList, const string& currentUser);
void loadGuestsFromFile(vector<Guest>& guestList, const string& currentUser);

#endif