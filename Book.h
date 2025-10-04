#pragma once
#include <string>
#include <iostream>

class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    bool isAvailable;

public:
    // Constructor to initialize a Book object
    Book(std::string title, std::string author, std::string isbn);

    // Function to display book details
    void display();

    // Getter functions to access private members
    std::string getIsbn() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    void setTitle(const std::string& newTitle);
    void setAuthor(const std::string& newAuthor);
    bool checkAvailability() const;

    void setStatus(bool status);
};