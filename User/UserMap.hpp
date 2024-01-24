#ifndef USERMAP_HPP
#define USERMAP_HPP

#include <map>
#include "User.hpp"

class UserMap {
private:
    std::map<int, User> userMap;

public:
    UserMap();
    ~UserMap();
    void addUser(int fd, User user);
    void deleteUser(int fd);
    User findUser(int fd) const;
    bool exists(int fd);
};

#endif