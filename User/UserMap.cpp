#include "UserMap.hpp"

#include <exception>

UserMap::UserMap() {}

UserMap::~UserMap() {}

void UserMap::addUser(int fd, User user) {
  std::pair<std::map<int, User>::iterator, bool> result;

  result = userMap.insert(std::make_pair(fd, user));
  if (!result.second) {
    throw std::invalid_argument("Cannot add user!! duplicate key exist");
  }
}

void UserMap::deleteUser(int fd) {
  std::map<int, User>::iterator result = userMap.find(fd);
  userMap.erase(result);
}

User UserMap::findUser(int fd) const {
  std::map<int, User>::const_iterator result = userMap.find(fd);

  if (result == userMap.end()) {
    throw std::invalid_argument("cannot find user");
  }
  return result->second;
}

bool UserMap::exists(int fd) {
  std::map<int, User>::const_iterator result = userMap.find(fd);

  return result != userMap.end();
}