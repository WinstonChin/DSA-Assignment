#include "Admin.h"
#include <fstream>

const int max_size = 2500;
Game games[max_size];
Member members[1000];

//constructor
Admin::Admin() {}

//forward declaration for merge function
void merge(Game games[], int first, int mid, int last);

//helper function to load members from csv
void loadMembers(Member members[], int& size) {
	string line;
	ifstream file;

	file.open("Members.csv");
	if (file.fail()) {
		cout << "Couldn't open file\n";
		return;
	}

	size = 0;
	getline(file, line); //skip header

	while (getline(file, line)) {
		if (line.empty()) continue;

		members[size] = Member(line);
		size++;
	}
	file.close();
}

//helper function to load games from csv
void loadGames(Game games[], int& size) {
	string line;
	ifstream file;

	file.open("GamesList.csv");
	if (file.fail()) {
		cout << "Couldn't open file\n";
		return;
	}
	
	size = 0;
	getline(file, line); //skip header
	
	while (getline(file, line)) {
		if (line.empty()) continue;
		
		const int MAX_FIELDS = 10; 
		string row[MAX_FIELDS];
		int fieldCount = 0;
		string value;
		bool inQuotes = false;

		// loop over characters in line
		for (size_t i = 0; i < line.length(); i++) {
			if (line[i] == '"') {
				inQuotes = !inQuotes;   //turns true for opening ", false for closing "
			}
			else if (line[i] == ',' && !inQuotes) { //split at commas and outside of quotes
				if (fieldCount < MAX_FIELDS) {
					row[fieldCount++] = value; //add value into row
				}
				value.clear(); //reset value to "" to store next field
			}
			else {
				value += line[i];
			}
		}
		if (fieldCount < MAX_FIELDS) {
			row[fieldCount++] = value; // last field
		}

		if (fieldCount >= 6) {
			if(row[7].empty()) {
				row[7] = "-1"; //set returnDate to -1 if empty
			}

			if(row[6] == "true") {
				Game game(row[0], stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]), true, stoi(row[7]));
				games[size++] = game;
				continue;
			}
			Game game(row[0], stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]), false, stoi(row[7]));
			games[size++] = game;
		}
	}
	file.close();
}

//helper function to store games into csv
void storeGames(Game games[],int size) {
	ofstream file;
	file.open("GamesList.csv");
	if (file.fail()) {
		cout << "Couldn't open file" << endl;
	}
	else {
		file << "name,minPlayers,maxPlayers,minTime,maxTime,year,isBorrowed,returnDate" << endl; //header
		for (int i = 0; i < size; i++) {
			string isBorrowed;
			if (games[i].getIsBorrowed()) {
				isBorrowed = "true";
			}
			else {
				isBorrowed = "false";
			}

			//if game name has a comma, enclose it in quotes
			if (games[i].getName().find(',') != string::npos) { //if found comma and havent reached the end of the string
				file << "\"" << games[i].getName() << "\"," << games[i].getMinPlayers() << "," << games[i].getMaxPlayers() << "," << games[i].getMinTime() << "," << games[i].getMaxTime() << "," << games[i].getYear() << "," << isBorrowed << "," << games[i].getReturnDate() << endl;
			}
			else {
				file << games[i].getName() << "," << games[i].getMinPlayers() << "," << games[i].getMaxPlayers() << "," << games[i].getMinTime() << "," << games[i].getMaxTime() << "," << games[i].getYear() << "," << isBorrowed << "," << games[i].getReturnDate() << endl;
			}
		}
		file.close();
	}
}

//mergesort helper functions
void mergesort(Game games[], int first, int last) {
	if (first < last) {    // more than 1 items 
		int mid = (first + last) / 2;    // index of midpoint
		mergesort(games, first, mid);    // sort left half
		mergesort(games, mid + 1, last); // sort right half
		merge(games, first, mid, last);  // merge the two halves
	}
}

void merge(Game games[], int first, int mid, int last) {
	Game tempArray[max_size];   // temporary array

	// initialize the local indexes to indicate the subarrays
	int first1 = first;     // beginning of first subarray
	int last1 = mid;        // end of first subarray
	int first2 = mid + 1;   // beginning of second subarray
	int last2 = last;       // end of second subarray

	// while neither subarray is empty, copy the smaller item into the temporary array
	int index = 0;    // next available location in tempArray
	while ((first1 <= last1) && (first2 <= last2)) {
		if (games[first1].getName() < games[first2].getName()) {
			tempArray[index] = games[first1];
			first1++;
		}
		else {
			tempArray[index] = games[first2];
			first2++;
		}
		index++;
	}

	// finish off the nonempty subarray to the back of tempArray
	// finish off the first subarray, if necessary
	for (; first1 <= last1; first1++, index++)
		tempArray[index] = games[first1];
	// finish off the second subarray, if necessary
	for (; first2 <= last2; first2++, index++)
		tempArray[index] = games[first2];

	// copy the result back into the original array
	for (int index2 = first, index = 0; index2 <= last; index++, index2++)
		games[index2] = tempArray[index];
}


//----------------Admin class functions------------------//
//add Game to the games list
void Admin::addGame(Game game) {
	//load games into an array
	int size = 0;
	loadGames(games, size);

	//add game to the back of the array
	if (size < max_size) {
		games[size] = game;
		size++;
	}

	//sort the array
	if (size > 1) {
		mergesort(games, 0, size - 1);
	}

	//write the array into GamesList.csv(rewrite the entire file)
	storeGames(games, size);

	cout << "Game added." << endl;
}

// remove a Game from games list
void Admin::removeGame(string name) {
	//load games into an array
	int size = 0;
	loadGames(games, size);

	int gameExist = 0;
	//find and remove the game from the array (including duplicates)
	for (int i = 0; i < size; i++) {
		if (games[i].getName() == name) {
			gameExist = 1;
			//shift all elements after the found game to the left
			for (int j = i; j < size-1; j++) {
				games[j] = games[j + 1];
			}
			size--; //decrease size
			i--; //check the game just shifted to position i
		}
	}
	if (gameExist == 0) {
		cout << "Game not found!\n";
		return;
	}

	//sort the array
	if (size > 1) {
		mergesort(games, 0, size - 1);
	}

	//write the array into GamesList.csv(rewrite the entire file)
	storeGames(games, size);
	cout << "Game removed.\n";
}

// add a Member to members list
void Admin::addMember(string username) {
	int size = 0;
	loadMembers(members, size);

	//check for same username
	for (int i = 0; i < size;i++) {
		if (members[i].getUsername() == username){
			cout << "Username already exists!\n";
			return;
		}
	}

	ofstream file;
	file.open("Members.csv", ios::app);
	if (file.fail()) {
		cout << "Couldn't open file\n";
	}
	else {
		file << username << endl;
		cout << "Member " << username << " added.\n";
		file.close();
	}
}

// display summary of borrowed and returned games
void Admin::displayBorrowedReturned() {
	int size = 0;
	loadGames(games, size);

	int borrowedCount = 0;
	cout << "Borrowed Games:\n";
	for(int i= 0; i < size; i++) {
		if (games[i].getIsBorrowed()) {
			cout << games[i].getName() << endl;
			borrowedCount++;
		}
	}
	if (borrowedCount == 0) {
		cout << "No games are currently borrowed.\n";
	}

	int returnedCount = 0;
	cout << "\nReturned Games (in the past 7 days):\n";
	for (int i = 0; i < size; i++) {
		int days = games[i].returnedDays();
		if (!games[i].getIsBorrowed() && games[i].getReturnDate() != -1) {
			if (days <= 7) {
				cout << games[i].getName() << endl;
				returnedCount++;
			}
		}
	}
	if (returnedCount == 0) {
		cout << "No games have been returned in the past 7 days.\n";
	}
}