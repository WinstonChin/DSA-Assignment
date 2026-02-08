#include "Member.h"
#include "Record.h"
#include <fstream>
#include <iostream>

const int max_records = 5000;
Record records[max_records];

void merge(Record records[], int first, int mid, int last);

/*
 * Default constructor for Member class
 * Initializes a Member object with an empty username
 * Input: None
 * Return: None
 */
Member::Member() : username("") {}  

/*
 * Parameterized constructor for Member class
 * Initializes a Member object with a specified username
 * Input: uname - username string for the member
 * Return: None
 */
Member::Member(const string& uname) : username(uname) {}  

/*
 * Sets the username of the member
 * Input: uname - username string to set
 * Return: None
 */
void Member::setUsername(const string& uname) { username = uname; }

/*
 * Gets the username of the member
 * Input: None
 * Return: string - the member's username
 */
string Member::getUsername() const { return username; }

//----------------Records functions------------------//
/*
 * Loads game records from Records.csv file into an array
 * Parses CSV fields including quoted player lists and timestamps
 * Creates Record objects with game name, winner, players, and timestamp
 * Input: records[] - array to store Record objects
 *        size - reference to integer that will store the number of records loaded
 *        memCounter - reference to integer for member count of last record
 * Return: None (updates size and memCounter parameters by reference)
 */
void loadRecords(Record records[], int &size, int &memCounter) {
  string line;
  ifstream file;

  file.open("Records.csv");
  if (file.fail()) {
    cout << "Couldn't open file" << endl;
    return;
  }

  getline(file, line); // skip header
  size = 0;            // Initialize size

  while (getline(file, line)) {
    if (line.empty())
      continue;

    const int MAX_FIELDS = 10;
    string row[MAX_FIELDS];
    int fieldCount = 0;
    string value;
    bool inQuotes = false;

    // Split at comma. When have quotes, save the whole quoted section
    for (size_t i = 0; i < line.length(); i++) {
      if (line[i] == '"') {
        inQuotes = !inQuotes; // Turns true for opening ", false for closing "
      } 
      else if (line[i] == ',' && !inQuotes) { // Split at commas outside of quotes
        if (fieldCount < MAX_FIELDS) {
          row[fieldCount++] = value; // Add value into row
        }
        value.clear(); // Reset value to "" to store next field
      } else {
        value += line[i];
      }
    }
    // Add the last field
    if (fieldCount < MAX_FIELDS) {
      row[fieldCount++] = value;
    }

    // Create Member objects from row[2] (the comma-separated player list)
    Member members[100];
    int localMemCounter = 0;

    if (fieldCount >= 3 && !row[2].empty()) {
      string memberList = row[2];
      string memberName;

      for (size_t i = 0; i < memberList.length() && localMemCounter < 100;
           i++) {
        if (memberList[i] == ',') {
          if (!memberName.empty()) {
            // Trim whitespace
            size_t start = memberName.find_first_not_of(" \t\r\n");
            size_t end = memberName.find_last_not_of(" \t\r\n");

            // Extract trimmed member name
            if (start != string::npos && end != string::npos) {
              memberName = memberName.substr(start, end - start + 1);
            }

            // Create Member object with username
            Member newMember;
            newMember.setUsername(memberName);
            members[localMemCounter++] = newMember;
          }
          memberName.clear();
        } else {
          memberName += memberList[i];
        }
      }

      // Add the last member name
      if (!memberName.empty() && localMemCounter < 100) {
        size_t start = memberName.find_first_not_of(" \t\r\n");
        size_t end = memberName.find_last_not_of(" \t\r\n");
        if (start != string::npos && end != string::npos) {
          memberName = memberName.substr(start, end - start + 1);
        }

        // Create Member object with username
        Member newMember;
        newMember.setUsername(memberName);
        members[localMemCounter++] = newMember;
      }
    }

    // Initialize remaining members with empty usernames
    for (int i = localMemCounter; i < 100; i++) {
      Member emptyMember;
      emptyMember.setUsername("");
      members[i] = emptyMember;
    }

    // Create record with the parsed data
    if (fieldCount >= 2 && size < max_records) {
      Record record(row[0], row[1], members, localMemCounter);
      // Set timestamp from CSV if available (field 3)
      if (fieldCount >= 4 && !row[3].empty()) {
        long long savedTimestamp = 0;
        // Convert string to long long
        for (int i = 0; row[3][i] != '\0'; i++) {
          if (row[3][i] >= '0' && row[3][i] <= '9') {
            savedTimestamp = savedTimestamp * 10 + (row[3][i] - '0');
          }
        }
        record.setTimestamp(savedTimestamp);
      }
      records[size++] = record;
    }
  }

  memCounter = 0; // Set memCounter after the loop based on the last record processed
  if (size > 0) {
    memCounter = records[size - 1].getMemberCount();
  }

  file.close();
}

/*
 * Saves game records from array into Records.csv file
 * Writes game name, winner, comma-separated player list, and timestamp
 * Handles game names with commas by enclosing them in quotes
 * Input: records[] - array of Record objects to save
 *        size - number of records in the array
 *        memCounter - member count (not actively used in save)
 * Return: None
 */
void saveRecords(Record records[], int size, int memCounter) {
  ofstream file;
  file.open("Records.csv");
  if (file.fail()) {
    cout << "Couldn't open file" << endl;
  } else {
    file << "gameName,winner,players,timestamp" << endl; // header
    for (int i = 0; i < size; i++) {
      if (records[i].getGame().find(',') != string::npos) { //if found comma and havent reached the end of the string
            file << "\"" << records[i].getGame() << "\",";
      }
      else {
			file << records[i].getGame() << ",";
      }

      file << records[i].getWinner() << ",\"";
      int currentMemberCount = records[i].getMemberCount();
      for (int n = 0; n < currentMemberCount; n++) {
        if (n > 0)
          file << ","; // Add comma between member names
        file << records[i].getMembers()[n].getUsername();
      }
      file << "\"," << records[i].getTimestamp() << endl;
    }
    file.close();
  }
}

/*
 * Recursively sorts an array of Record objects using merge sort algorithm
 * Sorts by game name first, then by timestamp for records with same game name
 * Input: records[] - array of Record objects to sort
 *        first - starting index of the subarray to sort
 *        last - ending index of the subarray to sort
 * Return: None (sorts array in place)
 */
void mergesort(Record records[], int first, int last) {
  if (first < last) {                  // more than 1 items
    int mid = (first + last) / 2;      // index of midpoint
    mergesort(records, first, mid);    // sort left half
    mergesort(records, mid + 1, last); // sort right half
    merge(records, first, mid, last);  // merge the two halves
  }
}

/*
 * Merges two sorted subarrays of Record objects into a single sorted array
 * Compares by game name first, then by timestamp if names are equal
 * Input: records[] - array containing the two subarrays to merge
 *        first - starting index of first subarray
 *        mid - ending index of first subarray
 *        last - ending index of second subarray
 * Return: None (merges in place into original array)
 */
void merge(Record records[], int first, int mid, int last) {
  // Allocate tempArray on the heap to avoid large stack usage
  Record* tempArray = new Record[max_records]; 

  // initialize the local indexes to indicate the subarrays
  int first1 = first;   // beginning of first subarray
  int last1 = mid;      // end of first subarray
  int first2 = mid + 1; // beginning of second subarray
  int last2 = last;     // end of second subarray

  // while neither subarray is empty, copy the smaller item into the temporary array
  int index = 0; // next available location in tempArray
  while ((first1 <= last1) && (first2 <= last2)) {
	// Compare by game name first, then by timestamp if game names are equal
    if (records[first1].getGame() < records[first2].getGame()) {
      tempArray[index] = records[first1];
      first1++;
    }
    else if (records[first1].getGame() > records[first2].getGame()) {
      tempArray[index] = records[first2];
      first2++;
    }
    else {
      // Game names are equal, compare by time
      if (records[first1].getTimestamp() <= records[first2].getTimestamp()) {
        tempArray[index] = records[first1];
        first1++;
      }
      else {
        tempArray[index] = records[first2];
        first2++;
      }
    }
    index++;
  }

  // finish off the nonempty subarray to the back of tempArray
  // finish off the first subarray, if necessary
  for (; first1 <= last1; first1++, index++)
    tempArray[index] = records[first1];
  // finish off the second subarray, if necessary
  for (; first2 <= last2; first2++, index++)
    tempArray[index] = records[first2];

  // copy the result back into the original array
  for (int index2 = first, index = 0; index2 <= last; index++, index2++)
    records[index2] = tempArray[index];

  // Free heap memory
  delete[] tempArray;
}

/*
 * Adds a new game record to the records list and saves to CSV
 * Loads existing records, adds new record, sorts by game name and timestamp, and saves
 * Input: record - Record object to be added to the list
 * Return: None
 */
void Member::addRecord(Record record) {
  int size = 0;
  int memCounter = 0;
  loadRecords(records, size, memCounter);

  if (size < max_records) {
    records[size++] = record;
  }

  if (size > 1) {
    mergesort(records, 0, size - 1);
  }

  saveRecords(records, size, memCounter);

  cout << "Record added." << endl;
}