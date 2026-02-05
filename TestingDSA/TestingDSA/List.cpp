#pragma once
#include "List.h"
#include <iostream>
using namespace std;

// constructor
List::List() {
  firstNode = nullptr;
  size = 0;
}

// destructor
List::~List() // code by copilot
{
  Node *currentNode = firstNode;
  while (currentNode != nullptr) {
    Node *nodeToDelete = currentNode;
    currentNode = currentNode->next;
    delete nodeToDelete;
  }
}

// add an item to the back of the list (append)
bool List::add(ItemType item) {
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
bool List::add(int index, ItemType item) {
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
void List::remove(int index) {
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
ItemType List::get(int index) {
  // check if index is valid
  bool success = (index >= 0) && (index < size);
  if (!success) {
    return "";
  }

  int counter = 0;
  Node *currentNode = firstNode;
  while (counter != index) {
    currentNode = currentNode->next;
    counter++;
  }
  return currentNode->item;
}

// check if the list is empty
bool List::isEmpty() {
  if (size == 0) {
    return true;
  }
  return false;
}

// check the size of the list
int List::getLength() { return size; }

// display all the items in the list
void List::print() {
  Node *currentNode = firstNode;
  while (currentNode != nullptr) {
    Node *nodeToPrint = currentNode;
    currentNode = currentNode->next;
    cout << nodeToPrint->item << "\n";
  }
}
