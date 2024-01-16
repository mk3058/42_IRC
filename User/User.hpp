#ifndef USER_HPP
#define USER_HPP

#include <map>
#include <string>

class Channel;

class User {
 private:
  int fd;
  std::map<std::string, Channel *> channels;
  std::string username;
  std::string nickname;

 public:
  User(std::string username, std::string nickname, int fd);
  void setNickname(std::string nickname);
  std::string getUsername() const;
  std::string getNickname() const;
  int getfd() const;
  void joinChannel(std::string channelName);
  void leaveChannel(std::string channelName);
};

#endif