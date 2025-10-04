#include "Library.h"
#include "UserFactory.h"
#include "Member.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>
#include <algorithm>

// Initialize the static instance to nullptr
Library* Library::instance = nullptr;

Library* Library::getInstance() {
    if (instance == nullptr) {
        instance = new Library();
    }
    return instance;
}

void Library::addBook() {
    std::string title, author, isbn;

    std::cout << "\n--- Add a New Book ---\n";
    std::cout << "Enter ISBN (or type '0' to cancel): ";
    std::cin >> isbn;
    if (isbn == "0") {
        std::cout << "Operation cancelled. Returning to menu." << std::endl;
        return; 
    }
    std::cout << "Enter Title: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter Author: ";
    std::getline(std::cin, author);

    books.push_back(Book(title, author, isbn));
    std::cout << "Book added successfully!\n";
}

void Library::addUser() {
    std::string name;
    int id;

    std::cout << "\n--- Add a New Member ---\n";
    std::cout << "Enter Member ID (or enter 0 to cancel): ";
    std::cin >> id;

    if (std::cin.fail()) {
        std::cout << "Invalid input. Operation cancelled." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    if (id == 0) {
        std::cout << "Operation cancelled. Returning to menu." << std::endl;
        return;
    }

    std::cout << "Enter Member Name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);

    auto newUser = UserFactory::createUser("Member", id, name);
    if (newUser) {
        users.push_back(std::move(newUser));
        std::cout << "Member '" << name << "' with ID " << id << " added successfully!" << std::endl;
    } else {
        std::cout << "Failed to create user." << std::endl;
    }
}

void Library::searchBook() {
    if (books.empty()) {
        std::cout << "The library is empty. No books to search for." << std::endl;
        return;
    }

    int choice;
    std::cout << "\n--- Search for a Book ---\n";
    std::cout << "1. Search by Title\n";
    std::cout << "2. Search by Author\n";
    std::cout << "0. Cancel\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cout << "Invalid input. Operation cancelled." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    if (choice == 0) {
        std::cout << "Operation cancelled. Returning to menu." << std::endl;
        return;
    }

    std::string searchTerm;
    bool found = false;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        
        case 1: // Search by Title
            std::cout << "Enter the title to search for: ";
            std::getline(std::cin, searchTerm);
            std::cout << "\nSearch Results:\n";
            for (auto& book : books) {
                if (book.getTitle().find(searchTerm) != std::string::npos) {
                    book.display();
                    std::cout << "--------------------" << std::endl;
                    found = true;
                }
            }
            break;

        case 2: // Search by Author
            std::cout << "Enter the author to search for: ";
            std::getline(std::cin, searchTerm);
            std::cout << "\nSearch Results:\n";
            for (auto& book : books) {
                if (book.getAuthor().find(searchTerm) != std::string::npos) {
                    book.display();
                    std::cout << "--------------------" << std::endl;
                    found = true;
                }
            }
            break;

        
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            return;
    }

    if (!found) {
        std::cout << "No books found matching your search term." << std::endl;
    }
}

void Library::borrowBook() {
    int memberId;
    std::string bookIsbn;

    std::cout << "\n--- Borrow a Book ---\n";
    std::cout << "Enter your Member ID (or enter 0 to cancel): ";
    std::cin >> memberId;

    if (std::cin.fail()) {
        std::cout << "Invalid input. Operation cancelled." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    if (memberId == 0) {
        std::cout << "Operation cancelled. Returning to menu." << std::endl;
        return;
    }
    std::cout << "Enter the ISBN of the book you want to borrow: ";
    std::cin >> bookIsbn;

    Book* bookToBorrow = nullptr;
    Member* borrowingMember = findMemberById(memberId);

    for (auto& book : books) {
        if (book.getIsbn() == bookIsbn) {
            bookToBorrow = &book;
            break;
        }
    }

    if (bookToBorrow == nullptr) {
        std::cout << "Error: Book with this ISBN not found." << std::endl;
        return;
    }
    if (!bookToBorrow->checkAvailability()) {
        std::cout << "Error: Book is currently not available." << std::endl;
        return;
    }
    if (borrowingMember == nullptr) {
        std::cout << "Error: Member with this ID not found." << std::endl;
        return;
    }
    if (borrowingMember->getBorrowedCount() >= 5) {
        std::cout << "Error: You have reached your borrow limit of 5 books." << std::endl;
        return;
    }

    bookToBorrow->setStatus(false); 
    borrowingMember->borrowBook(bookIsbn);

    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&currentTime);
    timeStr.pop_back();

    std::cout << "Book '" << bookToBorrow->getTitle() << "' successfully borrowed by " 
              << borrowingMember->getName() << " on " << timeStr << "." << std::endl;


    std::cout << "\nNOTE: You can keep this book for a maximum of " << MAX_BORROW_DAYS << " days." << std::endl;
    std::cout << "A fine of $" << std::fixed << std::setprecision(2) << FINE_PER_DAY << " per day will be charged for late returns." << std::endl;
}

void Library::returnBook() {
    int memberId;
    std::cout << "\n--- Return a Book (Librarian Panel) ---\n";
    std::cout << "Enter Member ID to view their borrowed books (or 0 to cancel): ";
    std::cin >> memberId;

    if (std::cin.fail() || memberId == 0) {
        std::cout << "Operation cancelled." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    Member* returningMember = findMemberById(memberId);
    if (returningMember == nullptr) {
        std::cout << "Error: Member with this ID not found." << std::endl;
        return;
    }

    const auto& borrowedList = returningMember->getBorrowedBooksList();
    if (borrowedList.empty()) {
        std::cout << "This member has not borrowed any books." << std::endl;
        return;
    }

    viewBorrowedBooks(memberId);
    
    std::string bookIsbn;
    std::cout << "\nEnter the ISBN of the book to be returned (or 0 to cancel): ";
    std::cin >> bookIsbn;

    if (bookIsbn == "0") {
        std::cout << "Operation cancelled." << std::endl;
        return;
    }

    if (!returningMember->hasBorrowedBook(bookIsbn)) {
        std::cout << "Error: This book was not borrowed by this member." << std::endl;
        return;
    }
    Book* bookToReturn = nullptr;
    for (auto& book : books) {
        if (book.getIsbn() == bookIsbn) {
            bookToReturn = &book;
            break;
        }
    }

    // --- Process the Return ---
    auto borrowDate = returningMember->getBorrowDate(bookIsbn);
    auto returnDate = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(returnDate - borrowDate);
    int daysBorrowed = duration.count() / 24;
    double fine = 0.0;
    if (daysBorrowed > MAX_BORROW_DAYS) {
        int lateDays = daysBorrowed - MAX_BORROW_DAYS;
        fine = lateDays * FINE_PER_DAY;
    }

    if (fine > 0) {
        std::cout << "\nALERT: This book is " << (daysBorrowed - MAX_BORROW_DAYS) << " day(s) late." << std::endl;
        std::cout << "A fine of $" << std::fixed << std::setprecision(2) << fine << " is due." << std::endl;
        
        char confirm;
        std::cout << "Has the fine been paid? (y/n): ";
        std::cin >> confirm;

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "Fine not paid. Book return has been cancelled." << std::endl;
            return;
        }
        std::cout << "Fine paid. Proceeding with return." << std::endl;
    }

    bookToReturn->setStatus(true);
    returningMember->returnBook(bookIsbn);

    std::cout << "Book '" << bookToReturn->getTitle() << "' successfully returned." << std::endl;
}

void Library::saveData() {
    // === Saving Books ===
    std::ofstream booksFile("books.txt");
    if (booksFile.is_open()) {
        for (const auto& book : books) {
            booksFile << book.getIsbn() << ","
                      << book.getTitle() << ","
                      << book.getAuthor() << ","
                      << book.checkAvailability() << "\n";
        }
        booksFile.close();
    }

    // === Saving Users ===
    std::ofstream usersFile("users.txt");
    if (usersFile.is_open()) {
        for (const auto& user : users) {
            if (dynamic_cast<Member*>(user.get())) {
                usersFile << "Member," << user->getId() << "," << user->getName() << "\n";
            }
        }
        usersFile.close();
    }

    // === FINAL STEP: Saving Borrow Records ===
    std::ofstream borrowFile("borrow_records.txt");
    if (borrowFile.is_open()) {
        for (const auto& user : users) {
            Member* member = dynamic_cast<Member*>(user.get());
            if (member) {
                for (const auto& record : member->getBorrowedBooksList()) {
                    auto timeSinceEpoch = record.borrowDate.time_since_epoch();
                    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch);
                    
                    borrowFile << member->getId() << ","
                               << record.bookIsbn << ","
                               << seconds.count() << "\n";
                }
            }
        }
        borrowFile.close();
    }
    std::cout << "All data saved successfully." << std::endl;
}

void Library::loadData() {
    // === Loading Books ===
    std::ifstream booksFile("books.txt");
    if (booksFile.is_open()) {
        std::string line;
        while (std::getline(booksFile, line)) {
            std::stringstream ss(line);
            std::string isbn, title, author, statusStr;
            std::getline(ss, isbn, ',');
            std::getline(ss, title, ',');
            std::getline(ss, author, ',');
            std::getline(ss, statusStr, ',');
            if (!isbn.empty()) {
                Book newBook(title, author, isbn);
                newBook.setStatus(statusStr == "1");
                books.push_back(newBook);
            }
        }
        booksFile.close();
    }

    // === Loading Users ===
    std::ifstream usersFile("users.txt");
    if (usersFile.is_open()) {
        std::string line;
        while (std::getline(usersFile, line)) {
            std::stringstream ss(line);
            std::string userType, idStr, name;
            std::getline(ss, userType, ',');
            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            if (!idStr.empty()) {
                int id = std::stoi(idStr);
                auto newUser = UserFactory::createUser(userType, id, name);
                if (newUser) {
                    users.push_back(std::move(newUser));
                }
            }
        }
        usersFile.close();
    }

    // === FINAL STEP: Loading Borrow Records ===
    std::ifstream borrowFile("borrow_records.txt");
    if (borrowFile.is_open()) {
        std::string line;
        while (std::getline(borrowFile, line)) {
            std::stringstream ss(line);
            std::string memberIdStr, bookIsbn, timeStr;
            std::getline(ss, memberIdStr, ',');
            std::getline(ss, bookIsbn, ',');
            std::getline(ss, timeStr, ',');
            if (!memberIdStr.empty()) {
                int memberId = std::stoi(memberIdStr);
                long long secondsCount = std::stoll(timeStr);
                Member* member = findMemberById(memberId);
                if (member) {
                    auto duration = std::chrono::seconds(secondsCount);
                    auto timePoint = std::chrono::system_clock::time_point(duration);
                    member->addBorrowedRecordFromFile({bookIsbn, timePoint});
                }
            }
        }
        borrowFile.close();
    }
    std::cout << "All data loaded successfully." << std::endl;
}

void Library::viewBorrowedBooks(int memberId) {
    Member* member = findMemberById(memberId);

    if (member == nullptr) {
        std::cout << "Error: Member with this ID not found." << std::endl;
        return;
    }

    const auto& borrowedList = member->getBorrowedBooksList();
    std::cout << "\n--- Books Borrowed by " << member->getName() << " (" << member->getBorrowedCount() << "/" << 5 << ") ---\n";

    if (borrowedList.empty()) {
        std::cout << "You have not borrowed any books." << std::endl;
        return;
    }

   
    std::cout << std::left 
              << std::setw(15) << "ISBN" 
              << std::setw(30) << "Title" 
              << std::setw(30) << "Borrowed On" 
              << "Fine\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    for (const auto& record : borrowedList) {
        Book* bookInfo = nullptr;
        for (auto& libBook : books) {
            if (libBook.getIsbn() == record.bookIsbn) {
                bookInfo = &libBook;
                break;
            }
        }

        if (bookInfo != nullptr) {
            std::time_t borrowTime = std::chrono::system_clock::to_time_t(record.borrowDate);
            std::string timeStr = std::ctime(&borrowTime);
            timeStr.pop_back();

            
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::hours>(now - record.borrowDate);
            int daysBorrowed = duration.count() / 24;
            double fine = 0.0;
            if (daysBorrowed > MAX_BORROW_DAYS) {
                fine = (daysBorrowed - MAX_BORROW_DAYS) * FINE_PER_DAY;
            }

           
            std::cout << std::left 
                      << std::setw(15) << bookInfo->getIsbn()
                      << std::setw(30) << bookInfo->getTitle()
                      << std::setw(30) << timeStr 
                      << "$" << std::fixed << std::setprecision(2) << fine << std::endl;
        }
    }
}

Member* Library::findMemberById(int memberId) {
    for (const auto& user : users) {
        if (user->getId() == memberId) {
            Member* member = dynamic_cast<Member*>(user.get());
            if (member) {
                return member;
            }
        }
    }
    return nullptr;
}

void Library::viewAllBorrowedBooks() {
    std::cout << "\n--- All Borrowed Books ---\n";
    bool anyBorrowed = false;

    // Table Header
    std::cout << std::left 
              << std::setw(15) << "ISBN" 
              << std::setw(25) << "Title" 
              << std::setw(15) << "Member Name" 
              << std::setw(10) << "Member ID" 
              << "Fine\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    for (const auto& book : books) {
        if (!book.checkAvailability()) {
            anyBorrowed = true;
            for (const auto& user : users) {
                Member* member = dynamic_cast<Member*>(user.get());
                if (member && member->hasBorrowedBook(book.getIsbn())) {
                    
                    
                    auto borrowDate = member->getBorrowDate(book.getIsbn());
                    auto now = std::chrono::system_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - borrowDate);
                    int daysBorrowed = duration.count() / 24;
                    double fine = 0.0;
                    if (daysBorrowed > MAX_BORROW_DAYS) {
                        fine = (daysBorrowed - MAX_BORROW_DAYS) * FINE_PER_DAY;
                    }

                    std::cout << std::left 
                              << std::setw(15) << book.getIsbn()
                              << std::setw(25) << book.getTitle()
                              << std::setw(15) << member->getName()
                              << std::setw(10) << member->getId()
                              << "$" << std::fixed << std::setprecision(2) << fine << std::endl;
                    break;
                }
            }
        }
    }

    if (!anyBorrowed) {
        std::cout << "No books are currently borrowed." << std::endl;
    }
}

void Library::viewAllBooks() {
    std::cout << "\n--- All Books in the Library ---\n";
    
    if (books.empty()) {
        std::cout << "The library is empty. No books to display." << std::endl;
        return;
    }

    // Table Header
    std::cout << std::left 
              << std::setw(15) << "ISBN" 
              << std::setw(30) << "Title" 
              << std::setw(25) << "Author" 
              << "Status\n";
    std::cout << "--------------------------------------------------------------------------\n";

   
    for (const auto& book : books) {
        std::cout << std::left 
                  << std::setw(15) << book.getIsbn()
                  << std::setw(30) << book.getTitle()
                  << std::setw(25) << book.getAuthor()
                  << (book.checkAvailability() ? "Available" : "Borrowed") << std::endl;
    }
}
void Library::updateBook() {
    std::string isbn;
    std::cout << "\nEnter ISBN of the book to update (or 0 to cancel): ";
    std::cin >> isbn;

    if (isbn == "0") {
        std::cout << "Operation cancelled." << std::endl;
        return;
    }

    Book* bookToUpdate = nullptr;
    for (auto& book : books) {
        if (book.getIsbn() == isbn) {
            bookToUpdate = &book;
            break;
        }
    }

    if (bookToUpdate == nullptr) {
        std::cout << "Error: Book with this ISBN not found." << std::endl;
        return;
    }

    std::cout << "\n--- Current Details ---\n";
    bookToUpdate->display();
    std::cout << "-----------------------\n";
    std::cout << "Enter new details. Press Enter to keep current value.\n";
    
    std::string newTitle, newAuthor;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::cout << "New Title (current: " << bookToUpdate->getTitle() << "): ";
    std::getline(std::cin, newTitle);

    std::cout << "New Author (current: " << bookToUpdate->getAuthor() << "): ";
    std::getline(std::cin, newAuthor);

    if (!newTitle.empty()) {
        bookToUpdate->setTitle(newTitle);
    }
    if (!newAuthor.empty()) {
        bookToUpdate->setAuthor(newAuthor);
    }

    std::cout << "Book details updated successfully!" << std::endl;
}

void Library::updateMember() {
    int memberId;
    std::cout << "\nEnter ID of the member to update (or 0 to cancel): ";
    std::cin >> memberId;

    if (std::cin.fail() || memberId == 0) {
        std::cout << "Operation cancelled." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    Member* memberToUpdate = findMemberById(memberId);
    if (memberToUpdate == nullptr) {
        std::cout << "Error: Member with this ID not found." << std::endl;
        return;
    }

    std::cout << "\nCurrent Name: " << memberToUpdate->getName() << std::endl;
    std::cout << "Enter new name (or press Enter to keep current): ";
    
    std::string newName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, newName);

    if (!newName.empty()) {
        memberToUpdate->setName(newName);
        std::cout << "Member name updated successfully!" << std::endl;
    } else {
        std::cout << "No changes made." << std::endl;
    }
}

void Library::removeBook() {
    std::string isbn;
    std::cout << "\nEnter ISBN of the book to remove (or 0 to cancel): ";
    std::cin >> isbn;

    if (isbn == "0") {
        std::cout << "Operation cancelled." << std::endl;
        return;
    }

    
    auto it = std::find_if(books.begin(), books.end(), [&](const Book& book) {
        return book.getIsbn() == isbn;
    });

    if (it == books.end()) {
        std::cout << "Error: Book with this ISBN not found." << std::endl;
        return;
    }

    
    if (!it->checkAvailability()) {
        std::cout << "Error: This book is currently borrowed and cannot be removed." << std::endl;
        return;
    }

    std::cout << "\n--- Book Details ---\n";
    it->display();
    std::cout << "--------------------\n";

    char confirm;
    std::cout << "Are you sure you want to remove this book? (y/n): ";
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        books.erase(it);
        std::cout << "Book removed successfully!" << std::endl;
    } else {
        std::cout << "Operation cancelled." << std::endl;
    }
}

void Library::removeMember() {
    int memberId;
    std::cout << "\nEnter ID of the member to remove (or 0 to cancel): ";
    std::cin >> memberId;

    if (std::cin.fail() || memberId == 0) {
        std::cout << "Operation cancelled." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    auto it = std::find_if(users.begin(), users.end(), [&](const std::unique_ptr<User>& user) {
        return user->getId() == memberId;
    });

    if (it == users.end()) {
        std::cout << "Error: Member with this ID not found." << std::endl;
        return;
    }

    Member* memberToRemove = dynamic_cast<Member*>(it->get());
    if (memberToRemove) {
        
        if (memberToRemove->getBorrowedCount() > 0) {
            std::cout << "Error: This member has " << memberToRemove->getBorrowedCount() 
                      << " book(s) borrowed and cannot be removed." << std::endl;
            return;
        }
    }

    std::cout << "\nMember Name: " << (*it)->getName() << " (ID: " << (*it)->getId() << ")" << std::endl;
    
    char confirm;
    std::cout << "Are you sure you want to remove this member? (y/n): ";
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        users.erase(it);
        std::cout << "Member removed successfully!" << std::endl;
    } else {
        std::cout << "Operation cancelled." << std::endl;
    }
}