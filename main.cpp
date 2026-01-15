#include <iostream>
using namespace std;

void adminMenu();
void memberMenu();

int main() {
    int choice;
    do {
        cout << "\n==== NPTTGC Board Game Management ====\n";
        cout << "1. Administrator Login\n";
        cout << "2. Member Login\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            adminMenu();  
            break;
        case 2:
            memberMenu(); 
            break;
        case 0:
            cout << "Goodbye\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
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
        switch (choice) {
        case 1:
            cout << "Adding board game...\n";
            break;
        case 2:
            cout << "Removing board game...\n";
            break;
        case 3:
            cout << "Adding member...\n";
            break;
        case 4:
            cout << "Displaying summary...\n";
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void memberMenu() {
    int choice;

    do {
        cout << "\n---- Member Menu ----\n";
        cout << "1. Borrow Game\n";
        cout << "2. Return Game\n";
        cout << "3. View My History\n";
        cout << "4. Rate Game\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Borrowing game...\n";
            break;
        case 2:
            cout << "Returning game...\n";
            break;
        case 3:
            cout << "Viewing history...\n";
            break;
        case 4:
            cout << "Rating game...\n";
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
