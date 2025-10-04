#pragma once
#include "User.h"
#include <string>
#include <memory> // For std::unique_ptr

class UserFactory {
public:
    // Static function to create different types of users
    static std::unique_ptr<User> createUser(const std::string& userType, int id, const std::string& name);
};