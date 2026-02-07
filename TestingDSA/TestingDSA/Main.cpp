#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "Admin.h"
#include "Game.h"
#include "Member.h"

using namespace std;

struct GameNode {
  char name[100];
  int minPlayers;
  int maxPlayers;
  int minTime;
  int maxTime;
  int year;
  bool isBorrowed;
  GameNode *next;
};

struct BorrowedNode {
  char name[100];
  int rating;
  BorrowedNode *next;
};

struct LLMember {
  char name[50];
  BorrowedNode *borrowed;
  BorrowedNode *returned;
};

GameNode *gameHead = nullptr;
GameNode *gameTail = nullptr;
LLMember currentMember = {"Guest", nullptr, nullptr};

// ================= LINKED LIST FUNCTIONS =================
void appendGame(GameNode *g) {
  g->next = nullptr;
  if (!gameHead) {
    gameHead = gameTail = g;
  } else {
    gameTail->next = g;
    gameTail = g;
  }
}

GameNode *findGameByName(const char *name) {
  GameNode *temp = gameHead;
  while (temp) {
    if (strcmp(temp->name, name) == 0)
      return temp;
    temp = temp->next;
  }
  return nullptr;
}

void addBorrowed(LLMember &m, const char *name, bool isReturned,
                 int rating = -1) {
  BorrowedNode *node = new BorrowedNode;
  strcpy_s(node->name, sizeof(node->name), name);
  node->rating = rating; // store rating if returning
  node->next = nullptr;

  BorrowedNode **head = isReturned ? &m.returned : &m.borrowed;
  node->next = *head;
  *head = node;
}

bool removeBorrowed(LLMember &m, const char *name, int *rating = nullptr) {
  BorrowedNode *temp = m.borrowed;
  BorrowedNode *prev = nullptr;

  while (temp) {
    if (strcmp(temp->name, name) == 0) {
      if (!prev)
        m.borrowed = temp->next;
      else
        prev->next = temp->next;
      if (rating)
        *rating = temp->rating;
      delete temp;
      return true;
    }
    prev = temp;
    temp = temp->next;
  }
  return false;
}

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

void loadGamesLL() {
  gameHead = nullptr;
  gameTail = nullptr;

  ifstream file("GamesList.csv");
  if (!file.is_open()) {
    cout << "ERROR: GamesList.csv not found!\n";
    return;
  }
  char line[200];
  file.getline(line, 200);
  while (file.getline(line, 200)) {
    if (strlen(line) == 0)
      continue;

    char fields[10][100];
    int fieldCount;
    parseCSVLine(line, fields, fieldCount);

    GameNode *g = new GameNode;
    g->isBorrowed = false;
    g->next = nullptr;
    strcpy_s(g->name, sizeof(g->name), fields[0]);
    g->minPlayers = atoi(fields[1]);
    g->maxPlayers = atoi(fields[2]);
    g->minTime = atoi(fields[3]);
    g->maxTime = atoi(fields[4]);
    g->year = atoi(fields[5]);

    appendGame(g);
  }
  file.close();
}

void showGames(bool onlyAvailable) {
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
}

void borrowGame() {
  showGames(true);
  cout << "\nEnter game name to borrow (or 0 to cancel): ";
  cin.ignore();
  char name[100];
  cin.getline(name, 100);
  if (strcmp(name, "0") == 0)
    return;

  GameNode *g = findGameByName(name);
  if (!g || g->isBorrowed) {
    cout << "Invalid game or already borrowed.\n";
    return;
  }

  g->isBorrowed = true;
  addBorrowed(currentMember, g->name, false);
  cout << "You borrowed " << g->name << endl;
}

void returnGame() {
  if (!currentMember.borrowed) {
    cout << "You have no borrowed games.\n";
    return;
  }

  cout << "\nYour borrowed games:\n";
  printBorrowedList(currentMember.borrowed);

  cout << "\nEnter game name to return: ";
  cin.ignore();
  char name[100];
  cin.getline(name, 100);

  int rating = 0;
  while (true) {
    cout << "Rate the game 1-10: ";
    cin >> rating;
    if (rating >= 1 && rating <= 10)
      break;
    cout << "Invalid rating. Please enter 1-10.\n";
  }

  if (!removeBorrowed(currentMember, name)) {
    cout << "Game not found.\n";
    return;
  }

  GameNode *g = findGameByName(name);
  if (g)
    g->isBorrowed = false;

  addBorrowed(currentMember, name, true, rating);
  cout << "Returned " << name << " with rating " << rating << "/10\n";
}

void memberHistory() {
  cout << "\nBorrowed:\n";
  printBorrowedList(currentMember.borrowed);
  cout << "\nReturned:\n";
  printBorrowedList(currentMember.returned);
}

void memberMenu() {
  int choice;
  do {
    cout << "\n---- Member Menu ----\n";
    cout << "1. Borrow Game\n";
    cout << "2. Return Game\n";
    cout << "3. View History\n";
    cout << "0. Logout\n";
    cout << "Choice: ";
    cin >> choice;
    switch (choice) {
    case 1:
      borrowGame();
      break;
    case 2:
      returnGame();
      break;
    case 3:
      memberHistory();
      break;
    }
  } while (choice != 0);
}

extern Game games[2500];
extern void loadGames(Game games[], int &size);
int gameSize = 0;
extern Member members[1000];
extern void loadMembers(Member members[], int &size);
int memberSize = 0;

void displayGames() {
  loadGames(games, gameSize);

  cout << "\n===== Games =====\n";
  for (int i = 0; i < gameSize; i++) {
    if (i > 0 && games[i].getName() == games[i - 1].getName())
      continue;
    cout << games[i].getName() << endl;
  }
}

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
    cin.ignore();

    if (choice == 1) {
      string name;
      int minPlayers, maxPlayers, minTime, maxTime, year;
      cout << "Enter game name: ";
      getline(cin, name);
      cout << "Min players: ";
      cin >> minPlayers;
      cout << "Max players: ";
      cin >> maxPlayers;
      while (maxPlayers < minPlayers) {
        cout << "Max players cannot be less than min players. Please "
                "re-enter.\n";
        cout << "Max players: ";
        cin >> maxPlayers;
      }
      cout << "Min time (minutes): ";
      cin >> minTime;
      cout << "Max time (minutes): ";
      cin >> maxTime;
      while (maxTime < minTime) {
        cout << "Max time cannot be less than min time. Please re-enter.\n";
        cin >> maxTime;
      }
      cout << "Year of release: ";
      cin >> year;
      while (year < 1950 || year > 2026) {
        cout << "Year must be between 1950 and 2026. Please re-enter.\n";
        cin >> year;
      }
      Game newGame(name, minPlayers, maxPlayers, minTime, maxTime, year, false,
                   -1);
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
      cin >> username;
      admin1.addMember(username);
    } else if (choice == 4) {
      cout << "\n=== Summary ===\n";
      admin1.displayBorrowedReturned();
    } else if (choice != 0) {
      cout << "Invalid input!\n";
    }
  }
}

int main() {
  loadGamesLL();

  int choice;
  do {
    cout << "\n==== NPTTGC Board Game Management ====\n";
    cout << "1. Administrator\n";
    cout << "2. Member\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1)
      adminMenu();
    else if (choice == 2)
      memberMenu();
    else {
      if (choice != 0)
        cout << "Invalid input!\n";
    }
  } while (choice != 0);

  return 0;
}
