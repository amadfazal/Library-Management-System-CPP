#include "Member.h"
#include <iostream>
#include <algorithm> // find_if, remove_if ke liye zaroori hai

// Constructor
Member::Member(int id, std::string name) : User(id, name) {}

// Menu Display Function
void Member::displayMenu() {
    std::cout << "\n--- Member Menu (" << name << ") ---\n";
    std::cout << "1. Search for a Book\n";
    std::cout << "2. Borrow a Book\n";
    std::cout << "3. View My Borrowed Books\n"; // <-- NAYA OPTION
    std::cout << "4. Logout\n";
}

// Updated Functions:

void Member::borrowBook(const std::string& bookIsbn) {
    // Naya variable 'borrowedBooks' istemal karein aur poora record add karein
    borrowedBooks.push_back({bookIsbn, std::chrono::system_clock::now()});
}

int Member::getBorrowedCount() {
    // Naya variable 'borrowedBooks' istemal karein
    return borrowedBooks.size();
}

void Member::returnBook(const std::string& bookIsbn) {
    // 'remove_if' aur 'erase' ka istemal karke record delete karein
    // Kyunke ab vector mein struct hai, humein is tarah remove karna hoga
    borrowedBooks.erase(
        std::remove_if(borrowedBooks.begin(), borrowedBooks.end(),
                       [&](const BorrowedBookRecord& record) {
                           return record.bookIsbn == bookIsbn;
                       }),
        borrowedBooks.end());
}

bool Member::hasBorrowedBook(const std::string& bookIsbn) {
    // 'find_if' ka istemal karke check karein ke record mojood hai
    auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(),
                           [&](const BorrowedBookRecord& record) {
                               return record.bookIsbn == bookIsbn;
                           });
    return it != borrowedBooks.end();
}

std::chrono::system_clock::time_point Member::getBorrowDate(const std::string& bookIsbn) {
    auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(),
                           [&](const BorrowedBookRecord& record) {
                               return record.bookIsbn == bookIsbn;
                           });
    if (it != borrowedBooks.end()) {
        return it->borrowDate;
    }
    // Agar kitaab na mile to ek minimum time point return karein
    return std::chrono::system_clock::time_point::min();
}
// Naye function ki implementation
const std::vector<BorrowedBookRecord>& Member::getBorrowedBooksList() const {
    return borrowedBooks;
}

void Member::addBorrowedRecordFromFile(const BorrowedBookRecord& record) {
    borrowedBooks.push_back(record);
}