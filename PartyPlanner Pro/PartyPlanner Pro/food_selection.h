#ifndef FOOD_SELECTION_H
#define FOOD_SELECTION_H

#include "common.h"
#include "guest_management.h"  // for Guest struct

// Function prototypes
int countFoodSelections(const std::string& foodString);
bool foodAlreadyExists(const std::string& foodList, const std::string& newFood);
std::string toLowerCase(const std::string& str);
bool guestNameMatches(const std::string& guestName, const std::string& inputName);
int countFoodPreference(const std::vector<Guest>& guestList, const std::string& foodItem);
std::vector<std::pair<std::string, int>> getAllFoodPreferences(const std::vector<Guest>& guestList);
void displayFoodMenu();
bool saveGuestData(const std::vector<Guest>& guestList, const std::string& currentUser);
void chooseFood(std::vector<Guest>& guestList);
void editGuestFood(std::vector<Guest>& guestList);
void viewGuestChoices(const std::vector<Guest>& guestList);
void foodMenu(std::vector<Guest>& guestList, const std::string& currentUser);

#endif