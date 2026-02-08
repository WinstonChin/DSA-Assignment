#pragma once
#include <string>
using namespace std;

// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475

class Record;  // Forward declaration

class Member
{
private:
	string username;
public:
	//constuctor
	Member();
	Member(const string& username);

	//get and set
	void setUsername(const string& username);
	string getUsername() const;

	// add a Record for a game
	// parameter: record - the Record object to be added
	static void addRecord(Record record);
};

