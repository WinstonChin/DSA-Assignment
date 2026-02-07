#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

#include "Member.h"
#include "Admin.h"
#include "Game.h"
#include "Record.h"

using namespace std;

// =======================================================
// Group 8 Team Members
// Verity Lee, S10268429
// Winston Chin, S10266664
// Asher Ng, S10267475
// =======================================================

struct GameNode {
    char name[100];
    int minPlayers;
    int maxPlayers;
    int minTime;
    int maxTime;
    int year;
    bool isBorrowed;
    GameNode* next;
};

struct BorrowedNode {
    char name[100];
    int rating;
    char reviewText[200];
    BorrowedNode* next;
};

struct LLMember {
    char name[50];
    BorrowedNode* borrowed;
    BorrowedNode* returned;
    LLMember* next;
};



GameNode* gameHead = nullptr;
GameNode* gameTail = nullptr;

LLMember* memberHead = nullptr;    
LLMember* currentMember = nullptr;


void appendGame(GameNode* g) {
    g->next = nullptr;
    if (!gameHead) {
        gameHead = gameTail = g;
    }
    else {
        gameTail->next = g;
        gameTail = g;
    }
}

GameNode* findGameByName(const char* name) {
    GameNode* temp = gameHead;
    while (temp) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return nullptr;
}


void addBorrowed(LLMember* m, const char* name, bool isReturned, int rating = -1, const char* review = "") {
    BorrowedNode* node = new BorrowedNode;
    strcpy_s(node->name, sizeof(node->name), name);
    node->rating = rating;
    strcpy_s(node->reviewText, sizeof(node->reviewText), review);
    node->next = nullptr;

    if (isReturned) {
        node->next = m->returned;
        m->returned = node;
    }
    else {
        node->next = m->borrowed;
        m->borrowed = node;
    }
}


bool removeBorrowed(LLMember* m, const char* name) {
    BorrowedNode* temp = m->borrowed;
    BorrowedNode* prev = nullptr;

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

void printBorrowedList(BorrowedNode* head) {
    BorrowedNode* temp = head;
    while (temp) {
        cout << "- " << temp->name;
        if (temp->rating != -1)
            cout << " [Rating: " << temp->rating << "/10]";
        cout << endl;
        temp = temp->next;
    }
}



bool parseCSVLine(char* line, char fields[][100], int& fieldCount) {
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
        }
        else {
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
        if (strlen(line) == 0) continue;

        char fields[10][100];
        int fieldCount;
        parseCSVLine(line, fields, fieldCount);

        GameNode* g = new GameNode;
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


void loadMembersLL() {
    ifstream file("Members.csv");
    if (!file.is_open()) {
        cout << "ERROR: Members.csv not found!\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        LLMember* m = new LLMember;
        strcpy_s(m->name, sizeof(m->name), line.c_str());
        m->borrowed = nullptr;
        m->returned = nullptr;
        m->next = memberHead;
        memberHead = m;
    }
    file.close();
}

void displayMembers() {
    cout << "\n===== Members =====\n";
    LLMember* temp = memberHead;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->name << endl;
        temp = temp->next;
    }
}

LLMember* loginMember() {
    displayMembers();
    cout << "\nEnter member username (0 to cancel): ";
    char name[50];
    cin.getline(name, 50);

    if (strcmp(name, "0") == 0) return nullptr;

    LLMember* temp = memberHead;
    while (temp) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }

    cout << "Member not found.\n";
    return nullptr;
}


void showGames(bool onlyAvailable) {
    cout << "\n===== Games =====\n";
    GameNode* temp = gameHead;
    int i = 1;

    while (temp) {
        if (!onlyAvailable || !temp->isBorrowed) {
            cout << i << ". " << temp->name;
            if (temp->isBorrowed) cout << " [BORROWED]";
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
    cin.getline(name, 100);

    if (strcmp(name, "0") == 0) return;

    GameNode* g = findGameByName(name);
    if (!g || g->isBorrowed) {
        cout << "Invalid game or already borrowed.\n";
        return;
    }

    g->isBorrowed = true;
    addBorrowed(currentMember, g->name, false);
    cout << "You borrowed " << g->name << endl;
}
void returnGame() {
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

    GameNode* g = findGameByName(name);
    if (g) g->isBorrowed = false;

    addBorrowed(currentMember, name, true, rating, review);
    cout << "Returned " << name << " with your review.\n";
}


void memberHistory() {
    cout << "\nBorrowed:\n";
    printBorrowedList(currentMember->borrowed);
    cout << "\nReturned:\n";
    printBorrowedList(currentMember->returned);
}

void showGameReviews(const char* gameName) {
    cout << "\n===== Reviews for " << gameName << " =====\n";

    LLMember* tempMember = memberHead;
    bool foundAny = false;

    while (tempMember) {
        BorrowedNode* r = tempMember->returned;
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

void memberMenu() {
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
        case 1: borrowGame(); break;
        case 2: returnGame(); break;
        case 3: memberHistory(); break;
        case 4: {
            showGames(false); // show all games
            cout << "\nEnter game name to see reviews: ";
            char name[100];
            cin.getline(name, 100);
            showGameReviews(name);
            break;
        }
        case 5:
            cout << "(Record feature unchanged)\n";
            break;
        }
    } while (choice != 0);
}



extern Game games[2500];
extern void loadGames(Game games[], int& size);
int gameSize = 0;

extern Member members[1000];
extern void loadMembers(Member members[], int& size);
int memberSize = 0;

void displayGames() {
    loadGames(games, gameSize);
    cout << "\n===== Games =====\n";
    for (int i = 0; i < gameSize; i++) {
        if (i > 0 && games[i].getName() == games[i - 1].getName()) continue;
        cout << games[i].getName() << endl;
    }
}

void adminMenu() {
    Admin admin1;
    int choice;
    do {
        cout << "\n---- Administrator Menu ----\n";
        cout << "1. Add Board Game\n";
        cout << "2. Remove Board Game\n";
        cout << "3. Add Member\n";
        cout << "4. Display Borrowed & Returned Summary\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1 || choice == 2 || choice == 3 || choice == 4)
            cout << "(Admin logic unchanged)\n";

    } while (choice != 0);
}



int main() {
    loadGamesLL();
    loadMembersLL();   

    int choice = -1;

    while (choice != 0) {
        cout << "\n==== NPTTGC Board Game Management ====\n";
        cout << "1. Administrator\n";
        cout << "2. Member\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            adminMenu();
        }
        else if (choice == 2) {
            currentMember = loginMember();   

            if (currentMember != nullptr) {
                memberMenu();             
                currentMember = nullptr;
            }
        }

        else if (choice != 0) {
            cout << "Invalid input!\n";
        }
    }

    return 0;
}

