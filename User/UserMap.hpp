#ifndef USERMAP_HPP
#define USERMAP_HPP


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
  void setUname(int fd, std::string username);
  User &findUser(int fd);
  User &findUser(std::string nickname);
  bool exists(int fd) const;
  bool exists(std::string nickname) const;
  int getUserMapCnt() { return userMap.size(); }
  std::vector<User *> findAllUsers();
};

#endif
