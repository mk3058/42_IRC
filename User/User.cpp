#include "User.hpp"

#include "Channel.hpp"

User::User()
    : fd(-1), username("Not initialized"), nickname("Not initialized") {}

User::User(std::string username, std::string nickname, int fd)
    : fd(fd), username(username), nickname(nickname) {}

void User::setNickname(std::string nickname) { this->nickname = nickname; }

std::string User::getUsername() const { return username; }

std::string User::getNickname() const { return nickname; }

int User::getfd() const { return fd; }

std::map<std::string, Channel *> &User::getChannels() { return this->channels; }