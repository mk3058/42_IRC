#include "User.hpp"

User::User(std::string username, std::string nickname, int fd)
    : username(username), nickname(nickname), fd(fd) {}

void User::setNickname(std::string nickname) { this->nickname = nickname; }

std::string User::getUsername() const { return username; }

std::string User::getNickname() const { return nickname; }

int User::getfd() const { return fd; }