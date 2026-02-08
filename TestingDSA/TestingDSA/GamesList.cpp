#pragma once
#include "GamesList.h"
#include "Game.h"
#include "Main.cpp"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
using namespace std;

// constructor
GamesList::GamesList() {
  firstNode = nullptr;
  size = 0;
}

// destructor
GamesList::~GamesList() // code by copilot
{
  Node *currentNode = firstNode;
  while (currentNode != nullptr) {
    Node *nodeToDelete = currentNode;
    currentNode = currentNode->next;
    delete nodeToDelete;
  }
}

// add an item to the back of the list (append)
bool GamesList::add(ItemType item) {
  // Node* newNode = new Node;
  // newNode->item = item;
  // newNode->next = nullptr;
  // Another way of creating a new node with item:
  Node *newNode = new Node{item, NULL};

  // check if list is empty
  bool emptyBool = isEmpty();
  if (emptyBool) {
    firstNode = newNode;
    // add 1 to size counter
    size++;
    return true;
  }

  Node *currentNode = firstNode;
  while (currentNode->next != NULL) {
    currentNode = currentNode->next;
  }
  currentNode->next = newNode;
  // add 1 to size counter
  size++;
  return true;
  // done!
}

// add an item at a specified position in the list (insert)
bool GamesList::add(int index, ItemType item) {
  // check if index is valid
  bool success = (index >= 0) && (index < size);
  if (!success) {
    return false;
  }

  // Node* newNode = new Node;
  // newNode->item = item;
  // newNode->next = nullptr;
  // Another way of creating a new node with item:
  Node *newNode = new Node{item, NULL};

  // check if list is empty
  bool emptyBool = isEmpty();
  if (emptyBool) {
    firstNode = newNode;
    // add 1 to size counter
    size++;
    return true;
  }

  int counter = 0;

  Node *currentNode = firstNode;
  while (counter != index) {
    currentNode = currentNode->next;
    counter++;
  }
  Node *nextNode = currentNode->next;
  currentNode->next = newNode;
  newNode->next = nextNode;
  // add 1 to size counter
  size++;
  return true;
  // done!
}

// remove an item at a specified position in the list
void GamesList::remove(int index) {
  // to check if index is valid
  bool success = (index >= 0) && (index < size);
  // to check if list is empty

  bool emptyBool = isEmpty();
  if (!success) {
    return void();
  } else if (emptyBool) {
    return void();
  } else {

    Node *nodeToDelete;

    int counter = 0;

    Node *currentNode = firstNode;
    // if node to delete is the first node
    if (counter == index) {
      nodeToDelete = firstNode;
      firstNode = firstNode->next;
      delete nodeToDelete;
      // minus 1 to size counter
      size--;
      return void();
    }

    // if node to delete is not the first node
    // traverse to the node before the node to delete
    while (counter != index - 1) {
      currentNode = currentNode->next;
      counter++;
    }
    nodeToDelete = currentNode->next;
    currentNode->next = currentNode->next->next;
    delete nodeToDelete;
    // minus 1 to size counter
    size--;
    // done!
  }
}

// get an item at a specified position of the list (retrieve)
ItemType GamesList::get(int index) {
  // check if index is valid
  bool success = (index >= 0) && (index < size);
  if (!success) {
    // if index is out or range, throw out of bounds error
    throw std::out_of_range("Index out of bounds!");
  }

  int counter = 0;
  Node *currentNode = firstNode;
  while (counter != index) {
    currentNode = currentNode->next;
    counter++;
  }
  return currentNode->item;
}

// get a Game by its name
ItemType GamesList::getGameByName(string name) {
  Node *currentNode = firstNode;
  while (currentNode != nullptr) {
    // check if name of currentNode matches name
    if (currentNode->item.getName() == name) {
      return currentNode->item;
    }
    // if name is not found,
    // continue to integrate
    currentNode = currentNode->next;
  }
  // return emtpy game
  Game game;
  return game;
}

// update a Game by its name
bool GamesList::updateGameByName(string name, Game game) {
  Node *currentNode = firstNode;
  while (currentNode != nullptr) {
    // check if name of currentNode matches name
    if (currentNode->item.getName() == name) {
      currentNode->item = game;
      return true;
    }
    // if name is not found,
    // continue to integrate
    currentNode = currentNode->next;
  }
  // return emtpy game
  return false;
}

// check if the list is empty
bool GamesList::isEmpty() {
  if (size == 0) {
    return true;
  }
  return false;
}

// check the size of the list
int GamesList::getLength() { return size; }

// display all the items in the list
void GamesList::print() {
  Node *currentNode = firstNode;
  while (currentNode != nullptr) {
    Node *nodeToPrint = currentNode;
    currentNode = currentNode->next;
    cout << nodeToPrint->item.getName() << "\n";
  }
}

// load games from CSV file
// Loads games from GamesList.csv into a linked list
// Parses each line and creates GameNode objects
// Handles borrow status and borrow date fields
void GamesList::loadGamesLL() {
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
    // parseCSVLine is from Main.cpp
    parseCSVLine(line, fields, fieldCount);

    // updating csv line field data
    // into GamesList
    // TODO: add a GamesList first brub

    // create a temp Game object to store data there
    // since gameList is a linked list nodes containing Game objects
    Game tempGame;
    tempGame.setIsBorrowed(false);
    tempGame.setBorrowDate(-1);
    strcpy_s(tempGame.getName(), sizeof(tempGame.getName()), fields[0]);
    tempGame.setMinPlayers(atoi(fields[1]));
    tempGame.setMaxPlayers(atoi(fields[2]));
    tempGame.setMinTime(atoi(fields[3]));
    tempGame.setMaxTime(atoi(fields[4]));
    tempGame.setYear(atoi(fields[5]));

    /*
    GameNode *g = new GameNode;
    g->isBorrowed = false;
    g->borrowDate = -1;
    g->next = nullptr;
    strcpy_s(g->name, sizeof(g->name), fields[0]);
    g->minPlayers = atoi(fields[1]);
    g->maxPlayers = atoi(fields[2]);
    g->minTime = atoi(fields[3]);
    g->maxTime = atoi(fields[4]);
    g->year = atoi(fields[5]);
    */

    // Handle isBorrowed field
    if (fieldCount >= 7) {
      tempGame.setIsBorrowed((strcmp(fields[6], "true") == 0));
      // g->isBorrowed = (strcmp(fields[6], "true") == 0);
    }

    // Handle borrowDate field (field 7)
    if (fieldCount >= 8) {
      tempGame.setBorrowDate(atoi(fields[7]));
      // g->borrowDate = atoi(fields[7]);
    }

    // add temp Game to gamesList
    add(tempGame);

    // appendGame(g);
  }
  file.close();
}

// save games to CSV file
// Saves the games linked list back to GamesList.csv
// Writes all game data including borrow status and dates
// Handles game names containing commas by enclosing in quotes
void GamesList::saveGamesLLToCSV() {
  ofstream file("GamesList.csv");
  if (!file.is_open()) {
    cout << "ERROR: Cannot write to GamesList.csv!\n";
    return;
  }

  // Write header
  file << "name,minPlayers,maxPlayers,minTime,maxTime,year,isBorrowed,"
          "borrowDate,returnDate"
       << endl;

  // set ptr temp to traverse through gamesList
  Node *temp = firstNode;
  // still making edits
  while (temp) {
    // set value into variables
    string name = temp->item.getName();
    int minPlayers = temp->item.getMinPlayers();
    int maxPlayers = temp->item.getMaxPlayers();
    int minTime = temp->item.getMinTime();
    int maxTime = temp->item.getMaxTime();
    int year = temp->item.getYear();
    bool isBorrowed = temp->item.getIsBorrowed();
    int borrowDate = temp->item.getBorrowDate();
    // Handle game names with commas
    if (strchr(name.c_str(), ',') != nullptr) {
      file << "\"" << name << "\"";
    } else {
      file << name;
    }

    file << "," << minPlayers << "," << maxPlayers << "," << minTime << ","
         << maxTime << "," << year << "," << (isBorrowed ? "true" : "false")
         << "," << borrowDate << ",-1"
         << endl; // returnDate always -1 for LL version

    temp = temp->next;
  }

  file.close();
}

// Allows a member to return a borrowed game
// Prompts for rating and review, updates game status and member's history
// Saves changes to CSV file
void GamesList::returnGame() {
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

  Node *g = findGameByName(name);
  if (g) {
    g->item.setIsBorrowed(false);
    g->item.setBorrowDate(-1); // Reset borrowDate when returned
  }

  addBorrowed(currentMember, name, true, rating, review);

  // Save the updated game list to CSV
  saveGamesLLToCSV();

  cout << "Returned " << name << " with your review.\n";
}
