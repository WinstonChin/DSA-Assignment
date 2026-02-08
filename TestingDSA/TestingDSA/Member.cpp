#include "Member.h"

/*
 * Default constructor for Member class
 * Initializes a Member object with an empty username
 * Input: None
 * Return: None
 */
Member::Member() : username("") {}  

/*
 * Parameterized constructor for Member class
 * Initializes a Member object with a specified username
 * Input: uname - username string for the member
 * Return: None
 */
Member::Member(const string& uname) : username(uname) {}  

/*
 * Sets the username of the member
 * Input: uname - username string to set
 * Return: None
 */
void Member::setUsername(const string& uname) { username = uname; }

/*
 * Gets the username of the member
 * Input: None
 * Return: string - the member's username
 */
string Member::getUsername() const { return username; }