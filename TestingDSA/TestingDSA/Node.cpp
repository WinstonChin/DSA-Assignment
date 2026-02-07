#include "Game.h"
#include <iostream>
using namespace std;

class Node {
public:
  Game data;
  Node *next;
  Node(Game game) {
    data = game;
    next = nullptr;
  }
};

// Function to split the linked list into two halves
// using the fast and slow node pointers to find the pointer
// at the halfway mark of the linked list
// Note: the linked list is a singly linked list
// i.e. there is no branching in the linked list

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
