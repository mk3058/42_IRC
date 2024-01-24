#ifndef USERMAP_HPP
#define USERMAP_HPP

#include <map>
#include <string>
#include <vector>

#include "User.hpp"

class UserMap {
 private:
  std::map<int, User> userMap;
  std::map<std::string, User> nicknameMap;

public:
    UserMap();
    ~UserMap();
    int getSize() const;
    void addUser(int fd, User user);
    void deleteUser(int fd);
    User findUser(int fd) const;
};

#endif
