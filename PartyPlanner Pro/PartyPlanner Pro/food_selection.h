#ifndef FOOD_SELECTION_H
#define FOOD_SELECTION_H

#include "common.h"
#include "guest_management.h"  // for Guest struct

// Function prototypes
void chooseFood(std::vector<Guest>& guestList);
void editGuestFood(std::vector<Guest>& guestList);
void viewGuestChoices(const std::vector<Guest>& guestList);
void calculateFoodBudget(const std::vector<Guest>& guestList);
void foodMenu(std::vector<Guest>& guestList);

#endif