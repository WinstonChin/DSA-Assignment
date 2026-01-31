#pragma once
#include <iostream>
#include <string>
#include "Game.h"
#include "Member.h"
using namespace std;

// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475

class Admin {
public:
	Admin();
	void addGame(Game game);
	void removeGame(string name);
	void addMember(string username);
	void displayBorrowedReturned();
};
