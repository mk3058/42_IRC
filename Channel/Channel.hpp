#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>

#include "UserMap.hpp"

#define UNLIMITED -1
#define DEFAULT 0b1
#define INVITE_ONLY 0b10
#define KET_REQURIE 0b100
#define USERMODE_TOPIC 0b10
#define USERMODE_SUPER 0b110

class Channel {
 private:
  std::string name;
  std::string topic;
  std::string password;
  int userLimit;
  int mode;
  UserMap users;
  UserMap banned_user;
  std::map<int, int> userPermit;

 public:
  Channel();
  Channel(std::string name, User user);
  void setTopic(std::string topic);
  void setMode(int mode);
  void setUserLimit(int limit);
  void kickUser(User user);
  const std::string& getName() const;
  const std::string& getTopic() const;
  const std::string& getPassword() const;
  int getUserLimit() const;
  int getMode() const;
  const UserMap& getUsers() const;
  const UserMap& getBannedUsers() const;
  const std::map<int, int>& getUserPermits() const;
};

#endif