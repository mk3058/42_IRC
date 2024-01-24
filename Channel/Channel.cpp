#include "Channel.hpp"

Channel::Channel() {
  this->name = "Not initialized";
  this->userLimit = UNLIMITED;
  this->mode = DEFAULT;
}

Channel::Channel(std::string name, User user) {
  this->name = name;
  this->users.addUser(user.getfd(), user);
  this->userLimit = UNLIMITED;
  this->mode = DEFAULT;
  this->userPermit.insert(std::make_pair(user.getfd(), USERMODE_SUPER));
}

void Channel::setTopic(std::string topic) { this->topic = topic; }
void Channel::setMode(int mode) {
  if (mode == DEFAULT || mode == INVITE_ONLY) this->mode = mode;
}

void Channel::setUserLimit(int limit) { this->userLimit = limit; }

void Channel::kickUser(User user) {
  this->users.deleteUser(user.getfd());
  this->banned_user.addUser(user.getfd(), user);
}

const std::string& Channel::getName() const { return name; }

const std::string& Channel::getTopic() const { return topic; }

const std::string& Channel::getPassword() const { return password; }

int Channel::getUserLimit() const { return userLimit; }

int Channel::getMode() const { return mode; }

const UserMap& Channel::getUsers() const { return users; }

const UserMap& Channel::getBannedUsers() const { return banned_user; }

const std::map<int, int>& Channel::getUserPermits() const { return userPermit; }