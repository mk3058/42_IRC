#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>

#include "UserMap.hpp"

class Channel {
 private:
  std::string name;
  std::string topic;
  std::string password;
  int userLimit;
  int mode;
  UserMap users;
  std::map<int, int> userPermit;

 public:
  Channel(std::string name, User user);
};

#endif