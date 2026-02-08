#include "GamesList.h"
#include <cstdlib>
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "Admin.h"
#include "Game.h"
#include "Member.h"
#include "Record.h"

using namespace std;

// =======================================================
// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475
// =======================================================

// Forward declaration
int getTodayYYYYMMDD();

// TODO: absorb GameNode and all its related functions into GamesList
// and replace it.
struct GameNode {
  char name[100];
  int minPlayers;
  int maxPlayers;
  int minTime;
  int maxTime;
  int year;
  bool isBorrowed;
  int borrowDate; // YYYYMMDD format
  GameNode *next;
};

struct BorrowedNode {
  char name[100];
  int rating;
  char reviewText[200];
  BorrowedNode *next;
};

struct LLMember {
  char name[50];
  BorrowedNode *borrowed;
  BorrowedNode *returned;
  LLMember *next;
};

// initialise game list global variable
GamesList *gamesList = new GamesList();

GameNode *gameHead = nullptr;
GameNode *gameTail = nullptr;

LLMember *memberHead = nullptr;
LLMember *currentMember = nullptr;

/*
 * Gets today's date as an integer in YYYYMMDD format
 * Used for tracking borrow dates
 * Input: None
 * Return: int - today's date as YYYYMMDD
 */
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

/*
 * Appends a game node to the end of the games linked list
 * Updates head and tail pointers as needed
 * Input: g - pointer to GameNode to append
 * Return: None
 */
// TODO: appendGame -> GamesList.add
void appendGame(GameNode *g) {
  g->next = nullptr;
  if (!gameHead) {
    gameHead = gameTail = g;
  } else {
    gameTail->next = g;
    gameTail = g;
  }
}

/*
 * Adds a borrowed or returned game to a member's history
 * Creates a new BorrowedNode and adds it to the appropriate list
 * Input: m - pointer to LLMember
 *        name - game name
 *        isReturned - true to add to returned list, false for borrowed list
 *        rating - game rating (1-10, -1 if not rated)
 *        review - review text (empty string if none)
 * Return: None
 */
void addBorrowed(LLMember *m, const char *name, bool isReturned,
                 int rating = -1, const char *review = "") {
  BorrowedNode *node = new BorrowedNode;
  strcpy_s(node->name, sizeof(node->name), name);
  node->rating = rating;
  strcpy_s(node->reviewText, sizeof(node->reviewText), review);
  node->next = nullptr;

  if (isReturned) {
    node->next = m->returned;
    m->returned = node;
  } else {
    node->next = m->borrowed;
    m->borrowed = node;
  }
}

/*
 * Removes a game from a member's borrowed list
 * Searches and deletes the node from the borrowed linked list
 * Input: m - pointer to LLMember
 *        name - game name to remove
 * Return: bool - true if game was found and removed, false otherwise
 */
bool removeBorrowed(LLMember *m, const char *name) {
  BorrowedNode *temp = m->borrowed;
  BorrowedNode *prev = nullptr;

  while (temp) {
    if (strcmp(temp->name, name) == 0) {
      if (!prev)
        m->borrowed = temp->next;
      else
        prev->next = temp->next;
      delete temp;
      return true;
    }
    prev = temp;
    temp = temp->next;
  }
  return false;
}

/*
 * Prints a list of borrowed or returned games
 * Displays game names and ratings if available
 * Input: head - pointer to first BorrowedNode in the list
 * Return: None
 */
void printBorrowedList(BorrowedNode *head) {
  BorrowedNode *temp = head;
  while (temp) {
    cout << "- " << temp->name;
    if (temp->rating != -1)
      cout << " [Rating: " << temp->rating << "/10]";
    cout << endl;
    temp = temp->next;
  }
}

/*
 * Parses a CSV line into individual fields
 * Handles quoted strings containing commas
 * Input: line - C-string containing the CSV line
 *        fields - 2D array to store parsed fields
 *        fieldCount - reference to store number of fields parsed
 * Return: bool - true if parsing succeeded
 */
bool parseCSVLine(char *line, char fields[][100], int &fieldCount) {
  fieldCount = 0;
  bool inQuotes = false;
  int idx = 0, f = 0;

  for (int i = 0; line[i] != '\0'; i++) {
    char c = line[i];
    if (c == '"') {
      inQuotes = !inQuotes;
      continue;
    }
    if (c == ',' && !inQuotes) {
      fields[f][idx] = '\0';
      f++;
      idx = 0;
    } else {
      fields[f][idx++] = c;
    }
  }
  fields[f][idx] = '\0';
  fieldCount = f + 1;
  return true;
}

// loadMembersLL by Winston was here
// it has been moved to GamesList.cpp

/*
 * Loads members from Members.csv into a linked list
 * Creates LLMember nodes for each username in the file
 * Input: None
 * Return: None (updates global memberHead pointer)
 */
void loadMembersLL() {
  ifstream file("Members.csv");
  if (!file.is_open()) {
    cout << "ERROR: Members.csv not found!\n";
    return;
  }

  string line;
  while (getline(file, line)) {
    if (line.empty())
      continue;

    LLMember *m = new LLMember;
    strcpy_s(m->name, sizeof(m->name), line.c_str());
    m->borrowed = nullptr;
    m->returned = nullptr;
    m->next = memberHead;
    memberHead = m;
  }
  file.close();
}

// saveGamesLLToCSV by Winston was here
// it has been moved to GamesList.cpp

/*
 * Displays a numbered list of all members
 * Shows member usernames with index numbers
 * Input: None
 * Return: None
 */
void displayMembers() {
  cout << "\n===== Members =====\n";
  LLMember *temp = memberHead;
  int i = 1;
  while (temp) {
    cout << i++ << ". " << temp->name << endl;
    temp = temp->next;
  }
}

/*
 * Prompts user to log in as a member by username or number
 * Validates input and returns the selected member
 * Input: None
 * Return: LLMember* - pointer to logged-in member, or nullptr if cancelled
 */
LLMember *loginMember() {
  displayMembers();
  cout << "\nEnter member username OR number (0 to cancel): ";

  char input[50];
  cin.getline(input, 50);

  if (strcmp(input, "0") == 0)
    return nullptr;

  bool isNumber = true;
  for (int i = 0; input[i] != '\0'; i++) {
    if (!isdigit(input[i])) {
      isNumber = false;
      break;
    }
  }

  if (isNumber) {
    int index = atoi(input);
    int currentIndex = 1;

    LLMember *temp = memberHead;
    while (temp) {
      if (currentIndex == index)
        return temp;
      currentIndex++;
      temp = temp->next;
    }

    cout << "Invalid member number.\n";
    return nullptr;
  }

  LLMember *temp = memberHead;
  while (temp) {
    if (strcmp(temp->name, input) == 0)
      return temp;
    temp = temp->next;
  }

  cout << "Member not found.\n";
  return nullptr;
}

/*
 * Displays a list of games
 * Can filter to show only available games or all games
 * Input: onlyAvailable - true to show only available games, false to show all
 * Return: None
 */
void showGames(bool onlyAvailable) {
  extern GamesList *gamesList;
  // TODO: add function
  gamesList->showGames(onlyAvailable);

  /*
  cout << "\n===== Games =====\n";
  GameNode *temp = gameHead;
  int i = 1;

  while (temp) {
    if (!onlyAvailable || !temp->isBorrowed) {
      cout << i << ". " << temp->name;
      if (temp->isBorrowed)
        cout << " [BORROWED]";
      cout << endl;
      i++;
    }
    temp = temp->next;
  }
  */
}

/*
 * Allows a member to borrow an available game
 * Updates game status, borrow date, and member's borrowed list
 * Saves changes to CSV file
 * Input: None (uses global currentMember)
 * Return: None
 */
void borrowGame() {
  extern GamesList *gamesList;
  showGames(true);
  cout << "\nEnter game name to borrow (or 0 to cancel): ";
  char name[100];
  cin.getline(name, 100);

  if (strcmp(name, "0") == 0)
    return;

  Game game;
  game = gamesList->getGameByName(name);
  if (game.getIsBorrowed()) {
    cout << "Invalid game or already borrowed.\n";
  }

  game.setIsBorrowed(true);
  game.setBorrowDate(getTodayYYYYMMDD());

  // update game in gamesList
  gamesList->updateGameByName(name, game);

  // Save the updated game list with borrowDate to CSV
  gamesList->saveGamesLLToCSV();

  cout << "You borrowed " << game.getName() << endl;
}

/*
 * Allows a member to return a borrowed game
 * Prompts for rating and review, updates game status and member's history
 * Saves changes to CSV file
 * Input: None (uses global currentMember)
 * Return: None
 */
void returnGame() {
  extern GamesList *gamesList;
  if (!currentMember->borrowed) {
    cout << "You have no borrowed games.\n";
    return;
  }

  cout << "\nYour borrowed games:\n";
  printBorrowedList(currentMember->borrowed);

  cout << "\nEnter game name to return: ";
  char name[100];
  cin.getline(name, 100);

  int rating;
  cout << "Rate the game 1-10: ";
  cin >> rating;
  cin.ignore();

  cout << "Write a review (optional): ";
  char review[200];
  cin.getline(review, 200);

  if (!removeBorrowed(currentMember, name)) {
    cout << "Game not found.\n";
    return;
  }

  addBorrowed(currentMember, name, true, rating, review);

  // Save the updated game list to CSV
  gamesList->saveGamesLLToCSV();

  cout << "Returned " << name << " with your review.\n";
}

/*
 * Displays the current member's borrow history
 * Shows both currently borrowed and previously returned games
 * Input: None (uses global currentMember)
 * Return: None
 */
void memberHistory() {
  cout << "\nBorrowed:\n";
  printBorrowedList(currentMember->borrowed);
  cout << "\nReturned:\n";
  printBorrowedList(currentMember->returned);
}

/*
 * Displays all reviews for a specific game
 * Searches through all members' returned games for matching reviews
 * Input: gameName - name of the game to show reviews for
 * Return: None
 */
void showGameReviews(const char *gameName) {
  cout << "\n===== Reviews for " << gameName << " =====\n";

  LLMember *tempMember = memberHead;
  bool foundAny = false;

  while (tempMember) {
    BorrowedNode *r = tempMember->returned;
    while (r) {
      if (strcmp(r->name, gameName) == 0) {
        foundAny = true;
        cout << "- " << tempMember->name << " rated " << r->rating << "/10";
        if (strlen(r->reviewText) > 0)
          cout << ": " << r->reviewText;
        cout << endl;
      }
      r = r->next;
    }
    tempMember = tempMember->next;
  }

  if (!foundAny)
    cout << "No reviews yet for this game.\n";
}

/*
 * Checks if a member username exists in Members.csv
 * Input: username - username to search for
 * Return: bool - true if member exists, false otherwise
 */
bool checkMember(const string &username) {
  ifstream file("Members.csv");
  if (!file.is_open()) {
    cout << "ERROR: Members.csv not found!\n";
    return false;
  }
  string line;
  bool found = false;
  while (getline(file, line)) {
    if (line == username) {
      found = true;
      break;
    }
  }
  file.close();
  return found;
}

/*
 * Checks if a game exists in GamesList.csv by name
 * Handles game names with or without quotes
 * Input: name - game name to search for
 * Return: bool - true if game exists, false otherwise
 */
bool findGame(string name) {
  ifstream file("GamesList.csv");
  if (!file.is_open()) {
    cout << "ERROR: GamesList.csv not found!\n";
    return false;
  }

  string line;
  getline(file, line); // skip header

  while (getline(file, line)) {
    if (line.empty())
      continue;

    char fields[10][100];
    int fieldCount;
    parseCSVLine(const_cast<char *>(line.c_str()), fields, fieldCount);

    if (fieldCount > 0 && strcmp(fields[0], name.c_str()) == 0) {
      file.close();
      return true;
    }
  }

  file.close();
  return false;
}

/*
 * Displays and handles the member menu interface
 * Provides options to borrow, return, view history, read reviews, and add
 * records Input: None (uses global currentMember) Return: None
 */
void memberMenu() {
  extern GamesList *gamesList;
  int choice;
  do {
    cout << "\n---- Member Menu (" << currentMember->name << ") ----\n";
    cout << "1. Borrow Game\n";
    cout << "2. Return Game\n";
    cout << "3. View History\n";
    cout << "4. Read Game Reviews\n";
    cout << "5. Add Record\n";
    cout << "0. Logout\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1:
      borrowGame();
      break;
    case 2:
      gamesList->returnGame();
      break;
    case 3:
      memberHistory();
      break;
    case 4: {
      showGames(false); // show all games
      cout << "\nEnter game name to see reviews: ";
      char name[100];
      cin.getline(name, 100);
      showGameReviews(name);
      break;
    }
    case 5: {
      cout << "Enter name of game: ";
      string gameName;
      getline(cin, gameName);
      bool gameExists = findGame(gameName);
      while (!gameExists) {
        cout << "Game does not exist. Please re-enter name of game: ";
        getline(cin, gameName);
        gameExists = findGame(gameName);
      }

      cout << "Enter name of winner: ";
      string winnerName;
      getline(cin, winnerName);
      bool memExist = checkMember(winnerName);
      while (!memExist) {
        cout << "Member does not exist. Please re-enter name of winner: ";
        getline(cin, winnerName);
        memExist = checkMember(winnerName);
      }

      cout << "Enter name of players one at a time, including yours. Enter 0 "
              "to quit: ";
      string player;
      getline(cin, player);
      bool exist = checkMember(player);
      Member members[100];
      int count = 0;
      while (player != "0") {
        while (!exist) {
          cout << "Member does not exist. Please re-enter name of player: ";
          getline(cin, player);
          exist = checkMember(player);
        }

        // Check if player already exists in the list
        bool alreadyAdded = false;
        for (int i = 0; i < count; i++) {
          if (members[i].getUsername() == player) {
            alreadyAdded = true;
            cout << "Player " << player << " has already been added.\n";
            break;
          }
        }
        if (!alreadyAdded) {
          Member m(player);
          members[count++] = m;
        }

        cout << "Enter name of players one at a time, including yours. Enter 0 "
                "to quit: ";
        getline(cin, player);
        exist = checkMember(player);
      }

      // Ensure winner is in members list
      bool winnerExists = false;
      for (int i = 0; i < count; i++) {
        if (members[i].getUsername() == winnerName) {
          winnerExists = true;
          break;
        }
      }
      if (!winnerExists) {
        members[count++] = Member(winnerName);
      }

      // Ensure current member is in members list (check again after adding
      // winner)
      bool currentExists = false;
      for (int i = 0; i < count; i++) {
        if (members[i].getUsername() == currentMember->name) {
          currentExists = true;
          break;
        }
      }
      if (!currentExists) {
        members[count++] = Member(currentMember->name);
      }

      // Create a Game object to call addRecord
      Game game(gameName, 0, 0, 0, 0, 0, false, -1, -1);
      Record recordInfo(gameName, winnerName, members, count);
      game.addRecord(recordInfo);
    } break;
    }
  } while (choice != 0);
}

// ================= ADMIN FUNCTIONS =================
extern Game games[2500];
extern void loadGames(Game games[], int &size);
int gameSize = 0;
extern Member members[1000];
extern void loadMembers(Member members[], int &size);
int memberSize = 0;

/*
 * Displays all games without duplicates
 * Loads games from array and shows unique game names
 * Input: None
 * Return: None
 */
void displayGames() {
  loadGames(games, gameSize);

  cout << "\n===== Games =====\n";
  for (int i = 0; i < gameSize; i++) {
    if (i > 0 && games[i].getName() == games[i - 1].getName())
      continue;
    cout << games[i].getName() << endl;
  }
}

/*
 * Displays and handles the administrator menu interface
 * Provides options to add/remove games, add members, and view summaries
 * Input: None
 * Return: None
 */
void adminMenu() {
  Admin admin1 = Admin();
  int choice = -1;
  while (choice != 0) {
    cout << "\n---- Administrator Menu ----\n";
    cout << "1. Add Board Game\n";
    cout << "2. Remove Board Game\n";
    cout << "3. Add Member\n";
    cout << "4. Display Borrowed & Returned Summary\n";
    cout << "0. Logout\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
      string name;
      int minPlayers, maxPlayers, minTime, maxTime, year;
      cout << "Enter game name: ";
      getline(cin, name);

      cout << "Min players: ";
      cin >> minPlayers;
      while (minPlayers < 1) {
        cout << "Min players must be at least 1. Please re-enter.\n";
        cout << "Min players: ";
        cin >> minPlayers;
      }
      while (minPlayers > 100) {
        cout << "Min players must be less than 100. Please re-enter.\n";
        cout << "Min players: ";
        cin >> minPlayers;
      }

      cout << "Max players: ";
      cin >> maxPlayers;
      while (maxPlayers < minPlayers) {
        cout << "Max players cannot be less than min players. Please "
                "re-enter.\n";
        cout << "Max players: ";
        cin >> maxPlayers;
      }
      while (maxPlayers < 2) {
        cout << "Max players must be at least 2. Please re-enter.\n";
        cout << "Max players: ";
        cin >> maxPlayers;
      }
      while (maxPlayers > 100) {
        cout << "Max players is 100. Please re-enter.\n";
        cout << "Max players: ";
        cin >> maxPlayers;
      }

      cout << "Min time (minutes): ";
      cin >> minTime;
      cout << "Max time (minutes): ";
      cin >> maxTime;
      while (maxTime < minTime) {
        cout << "Max time cannot be less than min time. Please re-enter.\n";
        cout << "Max time (minutes): ";
        cin >> maxTime;
      }
      cout << "Year of release: ";
      cin >> year;
      while (year < 1950 || year > 2026) {
        cout << "Year must be between 1950 and 2026. Please re-enter.\n";
        cout << "Year of release: ";
        cin >> year;
      }
      Game newGame(name, minPlayers, maxPlayers, minTime, maxTime, year, false,
                   -1, -1);
      admin1.addGame(newGame);
    } else if (choice == 2) {
      displayGames();
      string name;
      cout << "\nEnter game name to remove: ";
      getline(cin, name);
      admin1.removeGame(name);
    } else if (choice == 3) {
      cout << "Enter new member username: ";
      string username;
      getline(cin, username);
      admin1.addMember(username);
    } else if (choice == 4) {
      cout << "\n=== Summary ===\n";
      admin1.displayBorrowedReturned();
    } else if (choice != 0) {
      cout << "Invalid input!\n";
    }
  }
}

/*
 * Main entry point of the application
 * Loads data and presents main menu for administrator or member login
 * Input: None
 * Return: int - 0 on successful exit
 */
int main() {
  extern GamesList *gamesList;
  // load files
  gamesList->loadGamesLL();
  loadMembersLL();

  int choice = -1;

  while (choice != 0) {
    cout << "\n==== NPTTGC Board Game Management ====\n";
    cout << "1. Administrator\n";
    cout << "2. Member\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
      adminMenu();
    } else if (choice == 2) {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      currentMember = loginMember();

      if (currentMember != nullptr) {
        memberMenu();
        currentMember = nullptr;
      }
    } else if (choice != 0) {
      cout << "Invalid input! Please enter 1, 2, or 0.\n";
    }
  }

  return 0;
}
