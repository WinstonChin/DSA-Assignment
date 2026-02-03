#pragma once
#include <string>
using namespace std;

// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475

class Member
{
private:
	string username;
public:
	Member();
	Member(string username);
	void setUsername(string username);
	string getUsername();
};

