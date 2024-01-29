#ifndef NOTICE_HPP
#define NOTICE_HPP

#include <Server.hpp>
#include <string>

class Notice {
 public:
  void toChannel(std::string channelName, std::string message);
  void toUser(std::string userName, std::string message);
};

#endif