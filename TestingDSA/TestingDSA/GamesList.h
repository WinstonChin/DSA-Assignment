// List.h - - Specification of List ADT (implemented using Pointers)
#pragma once
#include "Game.h"
#include <iostream>
#include <string>
using namespace std;

typedef Game ItemType;

// About the GamesList class:
// The GamesList class is a list that stores Games,
// and will be responsible for all Games List related things, such as sorting
// etc.

class GamesList {
private:
  struct Node {
    ItemType item; // item
    Node *next;    // pointer pointing to next item
  };
  Node *firstNode; // point to the first item
  int size;        // number of items in the listS

  // Function to split the linked list into two halves
  // using the fast and slow node pointers to find the pointer
  // and splits the linked list at the centre,
  // and returning the pointer to the second half of the linked list
  // Note: the linked list is a singly linked list
  // i.e. there is no branching in the linked list
  // the split function splits the linked list
  Node *split(Node *head) {
    // first set both fast and slow node to head
    Node *fast = head;
    Node *slow = head;

    // move the fast pointer two steps each time
    // while the slow pointer moves one step,
    // until the fast pointer reaches the end of the linked list
    while (fast != nullptr && fast->next != nullptr) {
      fast = fast->next->next;
      if (fast != nullptr) {
        slow = slow->next;
      }
    }

    // split the list into two halves
    // by creating new linked list temp
    // and making slow the end of the current linked list
    // by setting slow->next to nullptr
    // so, current list = first half, temp = second half
    Node *temp = slow->next;
    slow->next = nullptr;
    // return temp, which is the second list
    return temp;
  }

  // Function to merge two sorted halves of a linked list together
  Node *merge(Node *first, Node *second) {
    // if either list is empty, return the other list
    if (first == nullptr)
      return second;
    if (second == nullptr)
      return first;

    // pick the smaller value between the first and second nodes
    // TODO: modify to sort by properties of game
    if (first->item.getName() < second->item.getName()) {
      // recursively merge the rest of the lists and
      // link the result to the current node
      first->next = merge(first->next, second);
      return first;
    } else {
      // recursively merge the rest of the lists
      // and link the result to the current node as well
      second->next = merge(first, second->next);
      return second;
    }
  }

  // Function to perform merge sort on a linked list
  Node *mergeSort(Node *head) {
    // Base case:
    // if the list is empty or has only one node,
    // it's already sorted and does not require any more sorting
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    // split the list into two halves
    Node *second = split(head);

    // recursively sort each half
    head = mergeSort(head);
    second = mergeSort(second);

    // merge the two sorted halves
    return merge(head, second);
  }

  // find game by name
  // it's here cause I need to use Nodes
  // Finds a game in the linked list by its name
  // Performs linear search through the games list
  // Input: name - game name to search for
  // Return: GameNode* - pointer to found game node, or nullptr if not found
  //
  // This is by Winston as well
  Node *findGameByName(const char *name) {
    Node *temp = firstNode;
    while (temp) {
      if (strcmp(temp->item.getName().c_str(), name) == 0)
        return temp;
      temp = temp->next;
    }
    return nullptr;
  }

  // Displays a list of games
  // Can filter to show only available games or all games
  // Input: onlyAvailable - true to show only available games, false to show all
  // Return: None
public:
  void showGames(bool onlyAvailable) {
    extern GamesList *gamesList;
    // TODO: add function

    cout << "\n===== Games =====\n";
    Node *temp = firstNode;
    int i = 1;

    while (temp) {
      if (!onlyAvailable || !temp->item.getIsBorrowed()) {
        cout << i << ". " << temp->item.getName();
        if (temp->item.getIsBorrowed())
          cout << " [BORROWED]";
        cout << endl;
        i++;
      }
      temp = temp->next;
    }
  }

public:
  // constructor
  GamesList();

  // destructor
  ~GamesList();

  // add an item to the back of the list (append)
  bool add(ItemType item);

  // add an item at a specified position in the list (insert)
  bool add(int index, ItemType item);

  // remove an item at a specified position in the list
  void remove(int index);

  // get an item at a specified position of the list (retrieve)
  ItemType get(int index);

  // get Game by name
  ItemType getGameByName(string name);

  // update Game by name
  bool updateGameByName(string name, Game game);

  // get linked list by
  // returning the pointer to the first Node

  // check if the list is empty
  bool isEmpty();

  // check the size of the list
  int getLength();

  // display all the items in the list
  void print();

  // GamesList sort function using mergesort
  void sort();

  // load games from CSV file
  void loadGamesLL();

  // save games to CSV file
  void saveGamesLLToCSV();

  // return game
  void returnGame();
};
