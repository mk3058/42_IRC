#include "UserMap.hpp"

#include <exception>

UserMap::UserMap() {}

UserMap::~UserMap() {}

void UserMap::addUser(int fd, User user) {
  std::pair<std::map<int, User>::iterator, bool> fdMapResult;
  std::pair<std::map<std::string, User>::iterator, bool> nicknameMapResult;

  fdMapResult = userMap.insert(std::make_pair(fd, user));
  nicknameMapResult =
      nicknameMap.insert(std::make_pair(user.getNickname(), user));
  if (!fdMapResult.second || !nicknameMapResult.second) {
    throw std::invalid_argument("Cannot add user!! duplicate key exist");
  }
}

void UserMap::deleteUser(int fd) {
  std::map<int, User>::iterator fdMapResult = userMap.find(fd);
  std::map<std::string, User>::iterator nicknameMapResult =
      nicknameMap.find(fdMapResult->second.getNickname());

  userMap.erase(fdMapResult);
  nicknameMap.erase(nicknameMapResult);
}

User UserMap::findUser(int fd) const {
  std::map<int, User>::const_iterator result = userMap.find(fd);

  if (result == userMap.end()) {
    throw std::invalid_argument("cannot find user");
  }
  return result->second;
}

int UserMap::getSize() const{
  return this->userMap.size();
}
