#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475

struct Game {
  char name[100];
  int minPlayers;
  int maxPlayers;
  int minTime;
  int maxTime;
  int year;
  bool isBorrowed;
  Game *next;
};

struct BorrowedNode {
  char name[100];
  BorrowedNode *next;
};

struct Member {
  char name[50];
  BorrowedNode *borrowed;
  BorrowedNode *returned;
};

Game *gameHead = nullptr;
Game *gameTail = nullptr;
Member currentMember = {"Guest", nullptr, nullptr};

void appendGame(Game *g) {
  g->next = nullptr;
  if (gameHead == nullptr) {
    gameHead = g;
    gameTail = g;
  } else {
    gameTail->next = g;
    gameTail = g;
  }
}

Game *findGameByName(const char *name) {
  Game *temp = gameHead;
  while (temp != nullptr) {
    if (strcmp(temp->name, name) == 0)
      return temp;
    temp = temp->next;
  }
  return nullptr;
}

void addBorrowed(Member &m, const char *name, bool isReturned) {
  BorrowedNode *node = new BorrowedNode;
  strcpy_s(node->name, sizeof(node->name), name);
  node->next = nullptr;
  BorrowedNode **head = (isReturned ? &m.returned : &m.borrowed);
  node->next = *head;
  *head = node;
}

bool removeBorrowed(Member &m, const char *name) {
  BorrowedNode *temp = m.borrowed;
  BorrowedNode *prev = nullptr;

  while (temp != nullptr) {
    if (strcmp(temp->name, name) == 0) {
      if (prev == nullptr)
        m.borrowed = temp->next;
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

void printBorrowedList(BorrowedNode *head) {
  BorrowedNode *temp = head;
  while (temp != nullptr) {
    cout << "- " << temp->name << endl;
    temp = temp->next;
  }
}

bool parseCSVLine(char *line, char fields[][100], int &fieldCount) {
  fieldCount = 0;
  bool inQuotes = false;
  int idx = 0;
  int f = 0;
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

void loadGames() {
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
    Game *g = new Game;
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
  Game *temp = gameHead;
  int i = 1;
  while (temp != nullptr) {
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
  char name[100];
  cin.ignore();
  cin.getline(name, 100);
  if (strcmp(name, "0") == 0)
    return;

  Game *g = findGameByName(name);
  if (g == nullptr || g->isBorrowed) {
    cout << "Invalid game or already borrowed.\n";
    return;
  }

  g->isBorrowed = true;
  addBorrowed(currentMember, g->name, false);
  cout << "You borrowed " << g->name << endl;
}

void returnGame() {
  if (currentMember.borrowed == nullptr) {
    cout << "You have no borrowed games.\n";
    return;
  }
  cout << "\nYour borrowed games:\n";
  printBorrowedList(currentMember.borrowed);
  cout << "\nEnter game name to return: ";
  char name[100];
  cin.ignore();
  cin.getline(name, 100);
  if (!removeBorrowed(currentMember, name)) {
    cout << "Game not found in borrowed list.\n";
    return;
  }
  Game *g = findGameByName(name);
  if (g != nullptr)
    g->isBorrowed = false;
  addBorrowed(currentMember, name, true);
  cout << "Returned " << name << endl;
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

void adminMenu() {
  int choice;
  do {
    cout << "\n---- Administrator Menu ----\n";
    cout << "1. Add Board Game\n";
    cout << "2. Remove Board Game\n";
    cout << "3. Display Summary\n";
    cout << "0. Logout\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();
    if (choice == 1) {
      Game *g = new Game;
      g->next = nullptr;
      g->isBorrowed = false;
      cout << "Enter game name: ";
      cin.getline(g->name, 100);
      cout << "Min players: ";
      cin >> g->minPlayers;
      cout << "Max players: ";
      cin >> g->maxPlayers;
      appendGame(g);
      cout << "Game added.\n";
    } else if (choice == 2) {
      cout << "\nEnter game name to remove: ";
      char name[100];
      cin.getline(name, 100);
      Game *temp = gameHead;
      Game *prev = nullptr;
      while (temp != nullptr && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
      }
      if (temp == nullptr)
        cout << "Game not found.\n";
      else {
        if (prev == nullptr)
          gameHead = temp->next;
        else
          prev->next = temp->next;
        if (temp == gameTail)
          gameTail = prev;
        delete temp;
        cout << "Game removed.\n";
      }
    } else if (choice == 3) {
      cout << "\n=== Summary ===\n";
      int total = 0, borrowed = 0;
      Game *temp = gameHead;
      while (temp != nullptr) {
        total++;
        if (temp->isBorrowed)
          borrowed++;
        temp = temp->next;
      }
      cout << "Total games: " << total << endl;
      cout << "Borrowed games: " << borrowed << endl;
      cout << "Available games: " << (total - borrowed) << endl;
    }
  } while (choice != 0);
}

int main() {
  loadGames();
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
    if (choice == 2)
      memberMenu();

  } while (choice != 0);

  return 0;
}
