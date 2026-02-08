#include "Game.h"
#include <fstream>

/*
 * Default constructor for Game class
 * Initializes a Game object with default values
 * Input: None
 * Return: None
 */
Game::Game()
    : name(""),
      minPlayers(0),
      maxPlayers(0),
      minTime(0),
      maxTime(0),
      year(0),
      isBorrowed(false),
      borrowDate(-1),
      returnDate(-1)
{}
/*
 * Parameterized constructor for Game class
 * Initializes a Game object with specified values
 * Input: n - game name
 *        minPlay - minimum number of players
 *        maxPlay - maximum number of players
 *        minT - minimum time to play (minutes)
 *        maxT - maximum time to play (minutes)
 *        y - year of release
 *        borrowed - whether the game is currently borrowed
 *        borrowD - borrow date in YYYYMMDD format
 *        returnD - return date in YYYYMMDD format (-1 if not returned)
 * Return: None
 */
Game::Game(string n, int minPlay, int maxPlay, int minT, int maxT, int y,
           bool borrowed, int borrowD, int returnD) {
  name = n;
  minPlayers = minPlay;
  maxPlayers = maxPlay;
  minTime = minT;
  maxTime = maxT;
  year = y;
  isBorrowed = borrowed;
  borrowDate = borrowD;
  returnDate = returnD;
}

/*
 * Sets the name of the game
 * Input: n - game name
 * Return: None
 */
void Game::setName(string n) { name = n; }
/*
 * Gets the name of the game
 * Input: None
 * Return: string - game name
 */
string Game::getName() { return name; }
/*
 * Sets the minimum number of players
 * Input: minPlay - minimum number of players
 * Return: None
 */
void Game::setMinPlayers(int minPlay) { minPlayers = minPlay; }
/*
 * Gets the minimum number of players
 * Input: None
 * Return: int - minimum number of players
 */
int Game::getMinPlayers() { return minPlayers; }
/*
 * Sets the maximum number of players
 * Input: maxPlay - maximum number of players
 * Return: None
 */
void Game::setMaxPlayers(int maxPlay) { maxPlayers = maxPlay; }
/*
 * Gets the maximum number of players
 * Input: None
 * Return: int - maximum number of players
 */
int Game::getMaxPlayers() { return maxPlayers; }
/*
 * Sets the minimum time to play
 * Input: minT - minimum time in minutes
 * Return: None
 */
void Game::setMinTime(int minT) { minTime = minT; }
/*
 * Gets the minimum time to play
 * Input: None
 * Return: int - minimum time in minutes
 */
int Game::getMinTime() { return minTime; }
/*
 * Sets the maximum time to play
 * Input: maxT - maximum time in minutes
 * Return: None
 */
void Game::setMaxTime(int maxT) { maxTime = maxT; }
/*
 * Gets the maximum time to play
 * Input: None
 * Return: int - maximum time in minutes
 */
int Game::getMaxTime() { return maxTime; }
/*
 * Sets the year of release
 * Input: y - year of release
 * Return: None
 */
void Game::setYear(int y) { year = y; }
/*
 * Gets the year of release
 * Input: None
 * Return: int - year of release
 */
int Game::getYear() { return year; }
/*
 * Sets whether the game is borrowed
 * Input: borrowed - true if borrowed, false otherwise
 * Return: None
 */
void Game::setIsBorrowed(bool borrowed) { isBorrowed = borrowed; }
/*
 * Gets whether the game is borrowed
 * Input: None
 * Return: bool - true if borrowed, false otherwise
 */
bool Game::getIsBorrowed() { return isBorrowed; }
/*
 * Sets the borrow date
 * Input: bDate - borrow date in YYYYMMDD format
 * Return: None
 */
void Game::setBorrowDate(int bDate) { borrowDate = bDate; }
/*
 * Gets the borrow date
 * Input: None
 * Return: int - borrow date in YYYYMMDD format
 */
int Game::getBorrowDate() { return borrowDate; }
/*
 * Sets the return date
 * Input: rDate - return date in YYYYMMDD format (-1 if not returned)
 * Return: None
 */
void Game::setReturnDate(int rDate) { returnDate = rDate; }
/*
 * Gets the return date
 * Input: None
 * Return: int - return date in YYYYMMDD format (-1 if not returned)
 */
int Game::getReturnDate() { return returnDate; }

/*
 * Gets today's date as an integer in YYYYMMDD format
 * Input: None
 * Return: int - today's date as YYYYMMDD
 */
static int getTodayYYYYMMDD() {
  auto now = std::chrono::system_clock::now();
  time_t tt = std::chrono::system_clock::to_time_t(now);
  tm local_tm;
  localtime_s(&local_tm, &tt);
  int year = local_tm.tm_year + 1900;
  int month = local_tm.tm_mon + 1;
  int day = local_tm.tm_mday;
  return year * 10000 + month * 100 + day;
}

/*
 * Converts a date in YYYYMMDD format to the number of days since epoch
 * Used for calculating date differences
 * Input: yyyymmdd - date in YYYYMMDD format (-1 for invalid/no date)
 * Return: int - number of days since epoch, or -1 if input is -1
 */
static int convertYYYYMMDDToDays(int yyyymmdd) {
  if (yyyymmdd == -1)
    return -1;
  int year = yyyymmdd / 10000;
  int month = (yyyymmdd / 100) % 100;
  int day = yyyymmdd % 100;

  std::tm tmDate = {};
  tmDate.tm_year = year - 1900; // years since 1900
  tmDate.tm_mon = month - 1;    // months since January
  tmDate.tm_mday = day;

  std::time_t timeSinceEpoch = std::mktime(&tmDate);
  auto tp = std::chrono::system_clock::from_time_t(timeSinceEpoch);
  auto hoursSinceEpoch =
      std::chrono::duration_cast<std::chrono::hours>(tp.time_since_epoch())
          .count();
  return static_cast<int>(hoursSinceEpoch / 24);
}

/*
 * Calculates the number of days a game has been borrowed
 * Computes the difference between today's date and the borrow date
 * Input: None
 * Return: int - number of days borrowed, or -1 if not borrowed
 */
int Game::borrowedDays() {
  if (borrowDate == -1)
    return -1;
  int todayDays = convertYYYYMMDDToDays(getTodayYYYYMMDD());
  int borrowDays = convertYYYYMMDDToDays(borrowDate);
  return todayDays - borrowDays;
}

/*
 * Calculates the number of days since a game was returned
 * Computes the difference between today's date and the return date
 * Input: None
 * Return: int - number of days since return, or -1 if not returned
 */
int Game::returnedDays() {
  if (returnDate == -1)
    return -1;
  int todayDays = convertYYYYMMDDToDays(getTodayYYYYMMDD());
  int returnDays = convertYYYYMMDDToDays(returnDate);
  return todayDays - returnDays;
}
