#include "UserMap.hpp"

#include <exception>

UserMap::UserMap() {}

UserMap::~UserMap() {}

void UserMap::addUser(int fd, User user) {
  std::pair<std::map<int, User>::iterator, bool> fdMapResult;
  std::pair<std::map<std::string, User *>::iterator, bool> nicknameMapResult;

  fdMapResult = userMap.insert(std::make_pair(fd, user));
  if (!fdMapResult.second) {
    throw std::invalid_argument("Cannot add user!! duplicate key exist");
  }
  nicknameMapResult = nicknameMap.insert(
      std::make_pair(user.getNickname(), &(userMap.find(fd)->second)));
  if (!nicknameMapResult.second) {
    throw std::invalid_argument("Cannot add user!! duplicate key exist");
  }
}

void UserMap::deleteUser(int fd) {
  std::map<int, User>::iterator fdMapResult = userMap.find(fd);
  if (fdMapResult == userMap.end()) {
    throw std::invalid_argument("Cannot delete user!! no such user");
    return;
  }

  std::map<std::string, User *>::iterator nicknameMapResult =
      nicknameMap.find(fdMapResult->second.getNickname());
  if (nicknameMapResult == nicknameMap.end()) {
    throw std::invalid_argument("Cannot delete user!! no such user");
    return;
  }

  userMap.erase(fdMapResult);
  nicknameMap.erase(nicknameMapResult);
}

void UserMap::setUsername(int fd, std::string username) {
  std::map<int, User>::iterator it = userMap.find(fd);
  it->second.setUsername(username);
}

User &UserMap::findUser(int fd) {
  std::map<int, User>::iterator result = userMap.find(fd);

  if (result == userMap.end()) {
    throw std::invalid_argument("cannot find user");
  }
  return result->second;
}

int UserMap::getSize() const { return this->userMap.size(); }

User &UserMap::findUser(std::string nickname) {
  std::map<std::string, User *>::iterator result = nicknameMap.find(nickname);

  if (result == nicknameMap.end()) {
    throw std::invalid_argument("cannot find user");
  }
  return *(result->second);
}

bool UserMap::exists(int fd) const {
  std::map<int, User>::const_iterator result = userMap.find(fd);

  return result != userMap.end();
}

bool UserMap::exists(std::string nickname) const {
  std::map<std::string, User *>::const_iterator result =
      nicknameMap.find(nickname);

  return result != nicknameMap.end();
}

std::vector<User *> UserMap::findAllUsers() {
  std::vector<User *> users;

  for (std::map<int, User>::iterator it = userMap.begin(); it != userMap.end();
       ++it) {
    users.push_back(&(it->second));
  }
  return users;
}