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
	// constructor
	Admin();

	// add a Game to the games list
	// parameters: game - Game to be added
	void addGame(Game game);

	// remove a Game from games list
	// parameters: name - name of the Game to be removed
	void removeGame(string name);

	// add a Member to members list
	// parameters: username - username of the Member to be added
	void addMember(string username);

	// display summary of borrowed and returned games
	void displayBorrowedReturned();
};
