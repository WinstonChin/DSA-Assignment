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

  // check if the list is empty
  bool isEmpty();

  // check the size of the list
  int getLength();

  // display all the items in the list
  void print();
};
