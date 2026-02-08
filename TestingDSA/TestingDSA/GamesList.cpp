#pragma once
#include "GamesList.h"
#include <cstddef>
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

// merge sort list
// using head instead so it is not mistaken for firstNode, though it is
// essentially the same
GamesList::Node *GamesList::*mergeSort(Node *head) {
  // Base case:
  // if the list is empty or has only one node,
  // it is sort, or has no need to be sorted further
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
