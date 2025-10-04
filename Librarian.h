#pragma once
#include "User.h"

class Librarian : public User {
public:
    Librarian(int id, std::string name);
    

    // Override the base class function
    void displayMenu() override;
};