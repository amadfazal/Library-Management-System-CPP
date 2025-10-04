#include "Book.h"

Book::Book(std::string title, std::string author, std::string isbn) {
    this->title = title;
    this->author = author;
    this->isbn = isbn;
    this->isAvailable = true; // By default, a new book is available
}

void Book::display() {
    std::cout << "ISBN: " << isbn << "\n"
              << "Title: " << title << "\n"
              << "Author: " << author << "\n"
              << "Status: " << (isAvailable ? "Available" : "Borrowed") << std::endl;
}

std::string Book::getIsbn() const { return isbn; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
bool Book::checkAvailability() const { return isAvailable; }

void Book::setStatus(bool status) {
    isAvailable = status;
}

void Book::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Book::setAuthor(const std::string& newAuthor) {
    author = newAuthor;
}