#include "UserFactory.h"
#include "Member.h"
#include "Librarian.h"

// Note: We are using smart pointers (unique_ptr) to avoid memory leaks.
std::unique_ptr<User> UserFactory::createUser(const std::string& userType, int id, const std::string& name) {
    if (userType == "Member") {
        return std::make_unique<Member>(id, name);
    }
    // We can add more user types here later if needed
    // else if (userType == "Librarian") {
    //     return std::make_unique<Librarian>(id, name);
    // }
    return nullptr;
}