#include "Member.h"

Member::Member() : username("") {}  
Member::Member(const string& uname) : username(uname) {}  
void Member::setUsername(const string& uname) { username = uname; }
string Member::getUsername() const { return username; }