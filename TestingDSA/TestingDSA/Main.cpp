#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Game {
    string name;
    int minPlayers;
    int maxPlayers;
    int minTime;
    int maxTime;
    int year;
    bool isBorrowed = false;
};

struct Member {
    string name;
    vector<string> borrowed;
    vector<string> returned;
};

vector<Game> games;
Member currentMember = { "Guest" };

void pause() {
    cout << "\nPress ENTER to continue...";
    cin.ignore();
    cin.get();
}

void loadGames() {
    games.clear();
    ifstream file("GamesList.csv");

    if (!file.is_open()) {
        cout << "ERROR: GamesList.csv not found!\n";
        cout << "Put it in the same folder as the .exe\n";
        pause();
        return;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Game g;
        string temp;

        getline(ss, g.name, ',');

        if (!getline(ss, temp, ',')) continue;
        g.minPlayers = atoi(temp.c_str());

        if (!getline(ss, temp, ',')) continue;
        g.maxPlayers = atoi(temp.c_str());

        if (!getline(ss, temp, ',')) continue;
        g.minTime = atoi(temp.c_str());

        if (!getline(ss, temp, ',')) continue;
        g.maxTime = atoi(temp.c_str());

        if (!getline(ss, temp, ',')) continue;
        g.year = atoi(temp.c_str());

        games.push_back(g);
    }

    file.close();

}

void showGames(bool onlyAvailable) {
    cout << "\n===== Games =====\n";
    for (int i = 0; i < games.size(); i++) {
        if (onlyAvailable && games[i].isBorrowed) continue;

        cout << i + 1 << ". " << games[i].name;
        if (games[i].isBorrowed) cout << " [BORROWED]";
        cout << endl;
    }
}

void borrowGame() {
    showGames(true);

    int choice;
    cout << "\nSelect game (0 to cancel): ";
    cin >> choice;

    if (choice == 0) return;
    if (choice < 1 || choice > games.size() || games[choice - 1].isBorrowed) {
        cout << "Invalid choice.\n";
        pause();
        return;
    }

    games[choice - 1].isBorrowed = true;
    currentMember.borrowed.push_back(games[choice - 1].name);

    cout << "You borrowed " << games[choice - 1].name << endl;
    pause();
}

void returnGame() {
    if (currentMember.borrowed.empty()) {
        cout << "You have no borrowed games.\n";
        pause();
        return;
    }

    cout << "\nYour borrowed games:\n";
    for (int i = 0; i < currentMember.borrowed.size(); i++)
        cout << i + 1 << ". " << currentMember.borrowed[i] << endl;

    int choice;
    cout << "Select game to return: ";
    cin >> choice;

    if (choice < 1 || choice > currentMember.borrowed.size()) {
        cout << "Invalid choice.\n";
        pause();
        return;
    }

    string name = currentMember.borrowed[choice - 1];
    currentMember.returned.push_back(name);
    currentMember.borrowed.erase(currentMember.borrowed.begin() + choice - 1);

    for (auto& g : games)
        if (g.name == name) g.isBorrowed = false;

    cout << "Returned " << name << endl;
    pause();
}

void memberHistory() {
    cout << "\nBorrowed:\n";
    for (auto& g : currentMember.borrowed) cout << "- " << g << endl;

    cout << "\nReturned:\n";
    for (auto& g : currentMember.returned) cout << "- " << g << endl;

    pause();
}

void memberMenu() {
    int choice;
    do {
        cout << "\n---- Member Menu ----\n";
        cout << "1. Borrow Game\n";
        cout << "2. Return Game\n";
        cout << "3. View History\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: borrowGame(); break;
        case 2: returnGame(); break;
        case 3: memberHistory(); break;
        }
    } while (choice != 0);
}

void adminMenu() {
    int choice;
    do {
        cout << "\n---- Administrator Menu ----\n";
        cout << "1. Add Board Game\n";
        cout << "2. Remove Board Game\n";
        cout << "3. Add Member\n";
        cout << "4. Display Summary\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Adding board game...\n";
            pause();
            break;

        case 2: {
            cout << "\n===== Games =====\n";
            for (int i = 0; i < games.size(); i++) {
                cout << i + 1 << ". " << games[i].name;
                if (games[i].isBorrowed) cout << " [BORROWED]";
                cout << endl;
            }
            int removeChoice;
            cout << "\nSelect game to remove (0 to cancel): ";
            cin >> removeChoice;
            cin.ignore();

            if (removeChoice == 0) {
                cout << "Canceling removal...\n";
            }
            else if (removeChoice < 1 || removeChoice > games.size()) {
                cout << "Invalid choice.\n";
            }
            else {
                cout << "Selected to remove: " << games[removeChoice - 1].name << endl;

            }
            pause();
            break;
        }

        case 3:
            cout << "Adding member...\n";
            pause();
            break;

        case 4:
            cout << "Displaying summary...\n";
            pause();
            break;

        case 0:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice.\n";
            pause();
        }
    } while (choice != 0);
}

int main() {
    loadGames();
    int choice;
    do {
        cout << "\n==== NPTTGC Board Game Management ====\n";
        cout << "1. Administrator\n";
        cout << "2. Member\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) adminMenu();
        if (choice == 2) memberMenu();

    } while (choice != 0);

    return 0;
}
