#pragma once
#include "User.h"
#include <vector>
#include <string>
#include <chrono> 


struct BorrowedBookRecord {
    std::string bookIsbn;
    std::chrono::system_clock::time_point borrowDate;
};


class Member : public User {
private:
    
    std::vector<BorrowedBookRecord> borrowedBooks;
    static const int MAX_BORROW_LIMIT = 5;

public:
    Member(int id, std::string name);

    void displayMenu() override;
    
    void borrowBook(const std::string& bookIsbn);
    int getBorrowedCount();
    void returnBook(const std::string& bookIsbn);
    bool hasBorrowedBook(const std::string& bookIsbn);
    std::chrono::system_clock::time_point getBorrowDate(const std::string& bookIsbn);

   
    const std::vector<BorrowedBookRecord>& getBorrowedBooksList() const;
    
    void addBorrowedRecordFromFile(const BorrowedBookRecord& record);
};