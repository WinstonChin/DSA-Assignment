#include "Member.h"

Member::Member() {}
Member::Member(string uname) { username = uname; }
void Member::setUsername(string uname) { username = uname; }
string Member::getUsername() { return username; }