#pragma once
#include <string>

class User {
protected:
    int id;
    std::string name;

public:
    User(int id, std::string name);
    virtual ~User() {} // Virtual destructor for base class

    
    virtual void displayMenu() = 0;
    void setName(const std::string& newName);

    int getId();
    std::string getName();
};