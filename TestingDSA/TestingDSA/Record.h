#pragma once
#include <iostream>
#include <string>
#include "Member.h"
using namespace std;

// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475

const int max_players = 100;

class Record
{
private:
	string game;
	string winner;
	Member members[max_players];
	int memberCount;  
	long long timestamp; //YYYYMMDDHHMMSS format

public:
	//constructor
	Record();
	Record(string game, string winner, Member members[], int memberCount);  

	//get and set
	void setGame(string game);
	string getGame();
	void setWinner(string winner);
	string getWinner();
	void setMembers(Member members[], int memberCount); 
	Member* getMembers();
	int getMemberCount();  
	void setTimestamp(long long timestamp);
	long long getTimestamp();
};

