#include "Record.h"
#include <chrono>

// Helper function to get current timestamp
long long getCurrentTimeStamp();

/*
 * Default constructor for Record class
 * Initializes a Record object with memberCount and timestamp set to 0
 * Input: None
 * Return: None
 */
Record::Record() : memberCount(0), timestamp(0) {}

/*
 * Parameterized constructor for Record class
 * Initializes a Record with game name, winner, player list, and current timestamp
 * Input: g - game name
 *        win - winner username
 *        mem[] - array of Member objects (players)
 *        count - number of members in the array
 * Return: None
 */
Record::Record(string g, string win, Member mem[], int count) {
	game = g;
	winner = win;
	memberCount = (count < max_players) ? count : max_players;  // Limit to max_mem
	for (int i = 0; i < memberCount; i++) {
		members[i] = mem[i];
	}
	// Initialize remaining members to empty
	for (int i = memberCount; i < max_players; i++) {
		members[i] = Member();
	}
	timestamp = getCurrentTimeStamp();
}

/*
 * Sets the members (players) for the record
 * Copies member objects from input array, limited to max_players
 * Input: mem[] - array of Member objects
 *        count - number of members in the array
 * Return: None
 */
void Record::setMembers(Member mem[], int count) {
	memberCount = (count < max_players) ? count : max_players;
	for (int i = 0; i < memberCount; i++) {
		this->members[i] = mem[i];
	}
}
/*
 * Sets the game name for the record
 * Input: g - game name
 * Return: None
 */
void Record::setGame(string g) { game = g; }
/*
 * Gets the game name from the record
 * Input: None
 * Return: string - game name
 */
string Record::getGame() { return game; }
/*
 * Sets the winner username for the record
 * Input: win - winner username
 * Return: None
 */
void Record::setWinner(string win) { winner = win; }
/*
 * Gets the winner username from the record
 * Input: None
 * Return: string - winner username
 */
string Record::getWinner() { return winner; }
/*
 * Gets the array of members (players) from the record
 * Input: None
 * Return: Member* - pointer to the members array
 */
Member* Record::getMembers() { return members; }
/*
 * Gets the number of members (players) in the record
 * Input: None
 * Return: int - number of members
 */
int Record::getMemberCount() { return memberCount; }
/*
 * Sets the timestamp for the record
 * Input: ts - timestamp in YYYYMMDDHHMMSS format
 * Return: None
 */
void Record::setTimestamp(long long ts) { timestamp = ts; }
/*
 * Gets the timestamp from the record
 * Input: None
 * Return: long long - timestamp in YYYYMMDDHHMMSS format
 */
long long Record::getTimestamp() { return timestamp; }

/*
 * Gets the current system time as a timestamp
 * Formats as YYYYMMDDHHMMSS (year, month, day, hour, minute, second)
 * Input: None
 * Return: long long - current timestamp in YYYYMMDDHHMMSS format
 */
long long getCurrentTimeStamp() {
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &tt);

	long long year = local_tm.tm_year + 1900;
	long long month = local_tm.tm_mon + 1;
	long long day = local_tm.tm_mday;
	long long hour = local_tm.tm_hour;
	long long minute = local_tm.tm_min;
	long long second = local_tm.tm_sec;

	return year * 10000000000LL + month * 100000000LL +
		day * 1000000LL + hour * 10000LL + minute * 100LL + second;
}