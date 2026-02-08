#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "Record.h"
using namespace std;

// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475

class Game
{
private:
	string name;
	int minPlayers;
	int maxPlayers;
	int minTime;
	int maxTime;
	int year;
	bool isBorrowed;
	int borrowDate; //stored as YYYYMMDD
	int returnDate; //-1 if not returned yet, else store actual date as YYYYMMDD

public:
	// constructor
	Game();
	Game(string name, int minPlayers, int maxPlayers, int minTime, int maxTime, int year, bool isBorrowed, int borrowDate, int returnDate);
	
	//  get and set
	void setName(string name);
	string getName();
	void setMinPlayers(int minPlayers);
	int getMinPlayers();
	void setMaxPlayers(int maxPlayers);
	int getMaxPlayers();
	void setMinTime(int minTime);
	int getMinTime();
	void setMaxTime(int maxTime);
	int getMaxTime();
	void setYear(int year);
	int getYear();
	void setIsBorrowed(bool isBorrowed);
	bool getIsBorrowed();
	void setBorrowDate(int borrowDate);
	int getBorrowDate();
	void setReturnDate(int returnDate);
	int getReturnDate();

	// calculates the number of days a game is borrowed from today's date
	// returns -1(if not borrowed) or the number of days between today's date and borrowDate
	int borrowedDays();

	// calculates the number of days a game is returned from today's date
	// returns -1(if not returned) or the number of days between today's date and returnDate 
	int returnedDays();

	// add a Record for the game
	// parameter: record - the Record object to be added
	void addRecord(Record record);
};
