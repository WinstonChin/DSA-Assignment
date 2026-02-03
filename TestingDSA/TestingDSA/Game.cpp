#include "Game.h"

//constructor 
Game::Game() {}
Game::Game(string n, int minPlay, int maxPlay, int minT, int maxT, int y, bool borrowed, int returnD) {
	name = n;
	minPlayers = minPlay;
	maxPlayers = maxPlay;
	minTime = minT;
	maxTime = maxT;
	year = y;
	rating = NULL;
	isBorrowed = borrowed;
	borrowDate = -1;
	returnDate = returnD;
	next = NULL;
}

// get and set
void Game::setName(string n) { name = n; }
string Game::getName() { return name; }
void Game::setMinPlayers(int minPlay) { minPlayers = minPlay; }
int Game::getMinPlayers() { return minPlayers; }
void Game::setMaxPlayers(int maxPlay) { maxPlayers = maxPlay; }
int Game::getMaxPlayers() { return maxPlayers; }
void Game::setMinTime(int minT) { minTime = minT; }
int Game::getMinTime() { return minTime; }
void Game::setMaxTime(int maxT) { maxTime = maxT; }
int Game::getMaxTime() { return maxTime; }
void Game::setYear(int y) { year = y; }
int Game::getYear() { return year; }
void Game::setIsBorrowed(bool borrowed) { isBorrowed = borrowed; }
bool Game::getIsBorrowed() { return isBorrowed; }
void Game::setBorrowDate(int bDate) { borrowDate = bDate; }
int Game::getBorrowDate() { return borrowDate; }
void Game::setReturnDate(int rDate) { returnDate = rDate; }
int Game::getReturnDate() { return returnDate; }

// Get today as YYYYMMDD integer
int getTodayYYYYMMDD() {
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &tt);
	int year = local_tm.tm_year + 1900;
	int month = local_tm.tm_mon + 1;
	int day = local_tm.tm_mday;
	return year * 10000 + month * 100 + day;
}

// Convert YYYYMMDD to days since a reference date (e.g., 2000-01-01)
int convertYYYYMMDDToDays(int yyyymmdd) {
	if (yyyymmdd == -1) return -1; 
	int year = yyyymmdd / 10000; 
	int month = (yyyymmdd / 100) % 100; 
	int day = yyyymmdd % 100; 

	std::tm tmDate = {}; 
	tmDate.tm_year = year - 1900; // years since 1900 
	tmDate.tm_mon = month - 1; // months since January 
	tmDate.tm_mday = day; 

	std::time_t timeSinceEpoch = std::mktime(&tmDate); 
	auto tp = std::chrono::system_clock::from_time_t(timeSinceEpoch); 
	auto hoursSinceEpoch = std::chrono::duration_cast<std::chrono::hours>( tp.time_since_epoch() ).count(); 
	return static_cast<int>(hoursSinceEpoch / 24); 
}

// calculates the number of days a game is borrowed from today's date
int Game::borrowedDays() {
	if (borrowDate == -1) return -1;
	int todayDays = convertYYYYMMDDToDays(getTodayYYYYMMDD());
	int borrowDays = convertYYYYMMDDToDays(borrowDate);
	return todayDays - borrowDays;
}

// calculates the number of days a game is returned from today's date
int Game::returnedDays() {
	if (returnDate == -1) return -1;
	int todayDays = convertYYYYMMDDToDays(getTodayYYYYMMDD());
	int returnDays = convertYYYYMMDDToDays(returnDate);
	return todayDays - returnDays;
}