#include "Librarian.h"
#include <iostream>

Librarian::Librarian(int id, std::string name) : User(id, name) {}

void Librarian::displayMenu() {
    std::cout << "\n--- Librarian Menu (" << name << ") ---\n";
    std::cout << " 1. Add Book\n";
    std::cout << " 2. Add Member\n";
    std::cout << " 3. Update Book\n";
    std::cout << " 4. Update Member\n";
    std::cout << " 5. Remove Book\n";
    std::cout << " 6. Remove Member\n";
    std::cout << " 7. Search for a Book\n";
    std::cout << " 8. View All Books\n";
    std::cout << " 9. View All Borrowed Books\n";
    std::cout << "10. Return a Book for a Member\n";
    std::cout << "11. Logout\n";
}