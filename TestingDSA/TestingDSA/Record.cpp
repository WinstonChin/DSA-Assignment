#include "Record.h"

//constructor
Record::Record() : memberCount(0) {}

Record::Record(string g, string win, Member mem[], int count) {
	game = g;
	winner = win;
	memberCount = (count < max_mem) ? count : max_mem;  // Limit to max_mem
	for (int i = 0; i < memberCount; i++) {
		members[i] = mem[i];
	}
	// Initialize remaining members to empty
	for (int i = memberCount; i < max_mem; i++) {
		members[i] = Member();
	}
}

void Record::setMembers(Member mem[], int count) {
	memberCount = (count < max_mem) ? count : max_mem;
	for (int i = 0; i < memberCount; i++) {
		this->members[i] = mem[i];
	}
}
void Record::setGame(string g) { game = g; }
string Record::getGame() { return game; }
void Record::setWinner(string win) { winner = win; }
string Record::getWinner() { return winner; }
Member* Record::getMembers() { return members; }
int Record::getMemberCount() { return memberCount; }