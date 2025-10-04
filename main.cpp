#include <iostream>
#include <limits>
#include "Library.h"
#include "Member.h"
#include "Librarian.h"

void showLibrarianMenuLoop(Library* library);
void showMemberMenuLoop(Library* library);

int main() {
    Library* library = Library::getInstance();
    library->loadData();

    std::cout << "Welcome to the Library Management System!" << std::endl;

    while (true) {
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Login as Librarian\n";
        std::cout << "2. Login as Member\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";

        int mainChoice;
        std::cin >> mainChoice;

        
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (mainChoice) {
            case 1:
                showLibrarianMenuLoop(library);
                break;
            case 2:
                showMemberMenuLoop(library);
                break;
            case 3:
                library->saveData();
                std::cout << "Exiting system. Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}

void showLibrarianMenuLoop(Library* library) {
    Librarian admin(1, "Admin");
    while (true) {
        admin.displayMenu();
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        
        switch (choice) {
            case 1: library->addBook(); break;
            case 2: library->addUser(); break;
            case 3: library->updateBook(); break;
            case 4: library->updateMember(); break;
            case 5: library->removeBook(); break;
            case 6: library->removeMember(); break;
            case 7: library->searchBook(); break;
            case 8: library->viewAllBooks(); break;
            case 9: library->viewAllBorrowedBooks(); break;
            case 10: library->returnBook(); break;
            case 11: return; // Logout
            default: std::cout << "Invalid choice!\n"; break;
        }
        std::cout << "\n----------------------------------------\n";
    }
}

void showMemberMenuLoop(Library* library) {
    int memberId;
    std::cout << "\nEnter your Member ID to login: ";
    std::cin >> memberId;

    if (std::cin.fail()) {
        std::cout << "Invalid ID format." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    
    Member* loggedInMember = library->findMemberById(memberId);

    
    if (loggedInMember == nullptr) {
        std::cout << "Login failed: Member with ID " << memberId << " not found." << std::endl;
        return;
    }

    
    std::cout << "Welcome, " << loggedInMember->getName() << "!" << std::endl;

    while (true) {
        
        loggedInMember->displayMenu();

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: library->searchBook(); break;
            case 2: library->borrowBook(); break;
            case 3: library->viewBorrowedBooks(memberId);
                break;
            case 4: 
                return; 
            default: std::cout << "Invalid choice!\n"; break;
        }
        std::cout << "\n----------------------------------------\n";
    }
}