#include "Game.h"
#include <fstream>

const int max_size = 5000;
Record records[max_size];

void merge(Record records[], int first, int mid, int last);

//constructor 
Game::Game() {}
Game::Game(string n, int minPlay, int maxPlay, int minT, int maxT, int y, bool borrowed, int returnD) {
	name = n;
	minPlayers = minPlay;
	maxPlayers = maxPlay;
	minTime = minT;
	maxTime = maxT;
	year = y;
	rating = NULL;
	isBorrowed = borrowed;
	borrowDate = -1;
	returnDate = returnD;
	next = NULL;
}

// get and set
void Game::setName(string n) { name = n; }
string Game::getName() { return name; }
void Game::setMinPlayers(int minPlay) { minPlayers = minPlay; }
int Game::getMinPlayers() { return minPlayers; }
void Game::setMaxPlayers(int maxPlay) { maxPlayers = maxPlay; }
int Game::getMaxPlayers() { return maxPlayers; }
void Game::setMinTime(int minT) { minTime = minT; }
int Game::getMinTime() { return minTime; }
void Game::setMaxTime(int maxT) { maxTime = maxT; }
int Game::getMaxTime() { return maxTime; }
void Game::setYear(int y) { year = y; }
int Game::getYear() { return year; }
void Game::setIsBorrowed(bool borrowed) { isBorrowed = borrowed; }
bool Game::getIsBorrowed() { return isBorrowed; }
void Game::setBorrowDate(int bDate) { borrowDate = bDate; }
int Game::getBorrowDate() { return borrowDate; }
void Game::setReturnDate(int rDate) { returnDate = rDate; }
int Game::getReturnDate() { return returnDate; }

// Get today as YYYYMMDD integer
int getTodayYYYYMMDD() {
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &tt);
	int year = local_tm.tm_year + 1900;
	int month = local_tm.tm_mon + 1;
	int day = local_tm.tm_mday;
	return year * 10000 + month * 100 + day;
}

// Convert YYYYMMDD to days since a reference date (e.g., 2000-01-01)
int convertYYYYMMDDToDays(int yyyymmdd) {
	if (yyyymmdd == -1) return -1; 
	int year = yyyymmdd / 10000; 
	int month = (yyyymmdd / 100) % 100; 
	int day = yyyymmdd % 100; 

	std::tm tmDate = {}; 
	tmDate.tm_year = year - 1900; // years since 1900 
	tmDate.tm_mon = month - 1; // months since January 
	tmDate.tm_mday = day; 

	std::time_t timeSinceEpoch = std::mktime(&tmDate); 
	auto tp = std::chrono::system_clock::from_time_t(timeSinceEpoch); 
	auto hoursSinceEpoch = std::chrono::duration_cast<std::chrono::hours>( tp.time_since_epoch() ).count(); 
	return static_cast<int>(hoursSinceEpoch / 24); 
}

// calculates the number of days a game is borrowed from today's date
int Game::borrowedDays() {
	if (borrowDate == -1) return -1;
	int todayDays = convertYYYYMMDDToDays(getTodayYYYYMMDD());
	int borrowDays = convertYYYYMMDDToDays(borrowDate);
	return todayDays - borrowDays;
}

// calculates the number of days a game is returned from today's date
int Game::returnedDays() {
	if (returnDate == -1) return -1;
	int todayDays = convertYYYYMMDDToDays(getTodayYYYYMMDD());
	int returnDays = convertYYYYMMDDToDays(returnDate);
	return todayDays - returnDays;
}



//----------------Records functions------------------//
void loadRecords(Record records[], int& size, int& memCounter) {
    string line;
    ifstream file;

    file.open("Records.csv");
    if (file.fail()) {
        cout << "Couldn't open file" << endl;
        return;
    }

    getline(file, line); //skip header
    size = 0; // Initialize size
    
    while (getline(file, line)) {
        if (line.empty()) continue;

        const int MAX_FIELDS = 10;
        string row[MAX_FIELDS];
        int fieldCount = 0;
        string value;
        bool inQuotes = false;

        // Split at comma. When have quotes, save the whole quoted section
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;   // Turns true for opening ", false for closing "
            }
            else if (line[i] == ',' && !inQuotes) { // Split at commas outside of quotes
                if (fieldCount < MAX_FIELDS) {
                    row[fieldCount++] = value; // Add value into row
                }
                value.clear(); // Reset value to "" to store next field
            }
            else {
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
            
            for (size_t i = 0; i < memberList.length() && localMemCounter < 100; i++) {
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
                }
                else {
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
        if (fieldCount >= 2 && size < max_size) {
            Record record(row[0], row[1], members, localMemCounter);
            records[size++] = record;
        }
    }
    
    memCounter = 0; // Set memCounter after the loop based on the last record processed
    if (size > 0) {
        memCounter = records[size - 1].getMemberCount();
    }
    
    file.close();
}

void saveRecords(Record records[], int size, int memCounter) {
    ofstream file;
    file.open("Records.csv");
    if (file.fail()) {
        cout << "Couldn't open file" << endl;
    }
    else {
        file << "gameName,winner,players,review" << endl; //header
        for (int i = 0; i < size; i++) {
            file << records[i].getGame() << "," << records[i].getWinner() << ",\"";
            int currentMemberCount = records[i].getMemberCount();
            for (int n = 0; n < currentMemberCount; n++) {
                if (n > 0) file << ",";  // Add comma between member names
                file << records[i].getMembers()[n].getUsername();
            }
            file << "\"," << endl;
        }
        file.close();
    }
}

//mergesort helper functions
void mergesort(Record records[], int first, int last) {
    if (first < last) {    // more than 1 items 
        int mid = (first + last) / 2;    // index of midpoint
        mergesort(records, first, mid);    // sort left half
        mergesort(records, mid + 1, last); // sort right half
        merge(records, first, mid, last);  // merge the two halves
    }
}

void merge(Record records[], int first, int mid, int last) {
    Record tempArray[max_size];   // temporary array

    // initialize the local indexes to indicate the subarrays
    int first1 = first;     // beginning of first subarray
    int last1 = mid;        // end of first subarray
    int first2 = mid + 1;   // beginning of second subarray
    int last2 = last;       // end of second subarray

    // while neither subarray is empty, copy the smaller item into the temporary array
    int index = 0;    // next available location in tempArray
    while ((first1 <= last1) && (first2 <= last2)) {
        if (records[first1].getGame() < records[first2].getGame()) {
            tempArray[index] = records[first1];
            first1++;
        }
        else {
            tempArray[index] = records[first2];
            first2++;
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
}

void Game::addRecord(Record record) {
    int size = 0;
	int memCounter = 0;
	loadRecords(records, size, memCounter);

    if (size < max_size) {
        records[size++] = record;
    }

    if (size > 1) {
        mergesort(records, 0, size - 1);
    }

	saveRecords(records, size, memCounter);

	cout << "Record added.\n";
}