#include "Admin.h"
#include <fstream>

//max games: 5000
//max members: 500
const int max_size = 5000;
const int max_mem = 500;
Game games[max_size];
Member members[max_mem];

/*
 * Default constructor for Admin class
 * Initializes an Admin object with no parameters
 * Input: None
 * Return: None
 */
Admin::Admin() {}

//forward declaration for merge function
void merge(Game games[], int first, int mid, int last);

/*
 * Loads member data from Members.csv file into an array
 * Reads each line from the CSV file and creates Member objects
 * Input: members[] - array to store Member objects
 *        size - reference to integer that will store the number of members loaded
 * Return: None (updates size parameter by reference)
 */
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

/*
 * Loads game data from GamesList.csv file into an array
 * Parses CSV fields including quoted strings with commas
 * Handles isBorrowed, borrowDate, and returnDate fields
 * Input: games[] - array to store Game objects
 *        size - reference to integer that will store the number of games loaded
 * Return: None (updates size parameter by reference)
 */
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
				row[7] = "-1"; //set borrowDate to -1 if empty
			}
			if (row[8].empty()) {
				row[8] = "-1"; //set returnDate to -1 if empty
			}

			if(row[6] == "true") {
				Game game(row[0], stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]), true, stoi(row[7]), stoi(row[8]));
				games[size++] = game;
				continue;
			}
			Game game(row[0], stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]), false, stoi(row[7]), stoi(row[8]));
			games[size++] = game;
		}
	}
	file.close();
}

/*
 * Saves game data from array into GamesList.csv file
 * Writes all game attributes including borrow status and dates
 * Handles game names with commas by enclosing them in quotes
 * Input: games[] - array of Game objects to save
 *        size - number of games in the array
 * Return: None
 */
void storeGames(Game games[],int size) {
	ofstream file;
	file.open("GamesList.csv");
	if (file.fail()) {
		cout << "Couldn't open file" << endl;
	}
	else {
		file << "name,minPlayers,maxPlayers,minTime,maxTime,year,isBorrowed,borrowDate,returnDate" << endl; //header
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
				file << "\"" << games[i].getName() << "\"," << games[i].getMinPlayers() << "," << games[i].getMaxPlayers() << "," << games[i].getMinTime() << "," << games[i].getMaxTime() << "," << games[i].getYear() << "," << isBorrowed << "," << games[i].getBorrowDate() << "," << games[i].getReturnDate() << endl;
			}
			else {
				file << games[i].getName() << "," << games[i].getMinPlayers() << "," << games[i].getMaxPlayers() << "," << games[i].getMinTime() << "," << games[i].getMaxTime() << "," << games[i].getYear() << "," << isBorrowed << "," << games[i].getBorrowDate() << "," << games[i].getReturnDate() << endl;
			}
		}
		file.close();
	}
}

/*
 * Recursively sorts an array of Game objects by name using merge sort algorithm
 * Divides the array into halves and merges them in sorted order
 * Input: games[] - array of Game objects to sort
 *        first - starting index of the subarray to sort
 *        last - ending index of the subarray to sort
 * Return: None (sorts array in place)
 */
void mergesort(Game games[], int first, int last) {
	if (first < last) {    // more than 1 items 
		int mid = (first + last) / 2;    // index of midpoint
		mergesort(games, first, mid);    // sort left half
		mergesort(games, mid + 1, last); // sort right half
		merge(games, first, mid, last);  // merge the two halves
	}
}

/*
 * Merges two sorted subarrays into a single sorted array
 * Compares game names alphabetically to determine merge order
 * Input: games[] - array containing the two subarrays to merge
 *        first - starting index of first subarray
 *        mid - ending index of first subarray
 *        last - ending index of second subarray
 * Return: None (merges in place into original array)
 */
void merge(Game games[], int first, int mid, int last) {
	// Allocate tempArray on the heap to avoid large stack usage
	Game* tempArray = new Game[max_size];

	int first1 = first;
	int last1 = mid;
	int first2 = mid + 1;
	int last2 = last;

	int index = 0;
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

	for (; first1 <= last1; first1++, index++)
		tempArray[index] = games[first1];
	for (; first2 <= last2; first2++, index++)
		tempArray[index] = games[first2];

	for (int index2 = first, index = 0; index2 <= last; index++, index2++)
		games[index2] = tempArray[index];

	// Free heap memory
	delete[] tempArray;
}


//----------------Admin class functions------------------//
/*
 * Adds a new game to the games list and saves to CSV
 * Loads existing games, adds new game, sorts alphabetically, and saves
 * Input: game - Game object to be added to the list
 * Return: None
 */
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

	cout << "Game added.\n";
}

/*
 * Removes all games with the specified name from the games list
 * Loads games, removes matching entries (including duplicates), sorts, and saves
 * Input: name - name of the game to remove
 * Return: None
 */
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

/*
 * Adds a new member to the members list
 * Checks for duplicate usernames before adding to Members.csv
 * Input: username - username of the new member to add
 * Return: None
 */
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

/*
 * Displays a summary of currently borrowed games and recently returned games
 * Shows borrowed games with number of days borrowed
 * Shows games returned within the past 7 days
 * Input: None
 * Return: None
 */
void Admin::displayBorrowedReturned() {
	int size = 0;
	loadGames(games, size);

	int borrowedCount = 0;
	cout << "Borrowed Games:\n";
	for(int i= 0; i < size; i++) {
		if (games[i].getIsBorrowed()) {
			int borrowedDays = games[i].borrowedDays();
			cout << games[i].getName() << " (borrowed " << borrowedDays << " days ago)" << endl;
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