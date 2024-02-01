#include "Channel.hpp"

Channel::Channel() {
  this->name = "Not initialized";
  this->userLimit = UNLIMITED;
  this->mode = DEFAULT;
}

Channel::Channel(std::string name) {
  this->name = name;
  this->userLimit = UNLIMITED;
  this->mode = DEFAULT;
}

void Channel::setTopic(std::string topic) { this->topic = topic; }
void Channel::setMode(int mode) { this->mode = mode; }

void Channel::setPassword(std::string password) { this->password = password; }

void Channel::setUserLimit(int limit) { this->userLimit = limit; }

void Channel::addUser(User user) {
  this->users.addUser(user.getfd(), user);
  if (this->users.getSize() == 1)
    this->userPermit.insert(std::make_pair(user.getfd(), USERMODE_SUPER));
  else
    this->userPermit.insert(std::make_pair(user.getfd(), DEFAULT));
}

void Channel::deleteUser(User user) {
  this->users.deleteUser(user.getfd());
  this->userPermit.erase(user.getfd());
}

const std::string& Channel::getName() const { return name; }

const std::string& Channel::getTopic() const { return topic; }

const std::string& Channel::getPassword() const { return password; }

int Channel::getUserLimit() const { return userLimit; }

int Channel::getMode() const { return mode; }

UserMap& Channel::getUsers() { return users; }

UserMap& Channel::getBannedUsers() { return banned_user; }

UserMap& Channel::getInvitedUsers() { return invited_user; }

std::map<int, int>& Channel::getUserPermits() { return userPermit; }
