#pragma once
#include "Book.h"
#include "User.h"
#include <vector>
#include <memory> // For std::unique_ptr

class Member; // Forward declaration

class Library {
private:
    std::vector<Book> books;
    std::vector<std::unique_ptr<User>> users; // Using smart pointers to handle polymorphism
    // Private constructor for Singleton
    Library() {}

    // Static instance for Singleton
    static Library* instance;

public:
    // Deleted copy constructor and assignment operator to prevent cloning
    Library(const Library&) = delete;
    void operator=(const Library&) = delete;

    // Public method to get the single instance
    static Library* getInstance();

     static const int MAX_BORROW_MINUTES = 1;
    static constexpr double FINE_PER_MINUTE = 1.0;

    // Core Library Functions
    void addBook();
    void addUser();
    void searchBook();
    void borrowBook();
    void returnBook();
    void saveData();
    void loadData();
    void viewAllBooks();
    void updateBook();
    void updateMember();
    void removeBook();
    void removeMember();

    void viewBorrowedBooks(int memberId);
    Member* findMemberById(int memberId);
    void viewAllBorrowedBooks();
};