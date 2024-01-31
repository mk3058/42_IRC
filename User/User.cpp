#include "User.hpp"

#include <sstream>

#include "Channel.hpp"

User::User()
    : fd(-1), username("Not initialized"), nickname("Not initialized") {}

User::User(int fd) : fd(fd) {
  std::stringstream ss;
  ss << fd;
  nickname = ss.str();
}

User::User(std::string username, std::string nickname, int fd)
    : fd(fd), username(username), nickname(nickname) {}

void User::setUsername(std::string username) { this->username = username; }

void User::setNickname(std::string nickname) { this->nickname = nickname; }

std::string User::getUsername() const { return username; }

std::string User::getNickname() const { return nickname; }

int User::getfd() const { return fd; }

std::map<std::string, Channel *> &User::getChannels() { return this->channels; }

void User::joinChannel(std::string channelName, Channel *channel) {
  this->channels.insert(make_pair(channelName, channel));
}

void User::leaveChannel(std::string channelName) {
  this->channels.erase(channelName);
}