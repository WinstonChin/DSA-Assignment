#include <iostream>
using namespace std;

class Node {
public:
  int data;
  Node *next;
  Node(int value) {
    data = value;
    next = nullptr;
  }
};

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
  if (first->data < second->data) {
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

// Function to print out the list
void printList(Node *head) {
  Node *current = head;
  while (current != nullptr) {
    cout << current->data << " ";
    if (current->next) {
      cout << "-> ";
    }
    current = current->next;
  }
  cout << endl;
}

/*
int main() {
  // create hard coded linked list
  Node *head = new Node(9);
  head->next = new Node(8);
  head->next->next = new Node(5);
  head->next->next->next = new Node(2);
  head->next->next->next->next = new Node(7);

  head = mergeSort(head);
  printList(head);

  return 0;
}
*/
