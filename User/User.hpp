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
  User();
  User(int fd);
  User(std::string username, std::string nickname, int fd);
  void setNickname(std::string nickname);
  void setUsername(std::string username);
  std::string getUsername() const;
  std::string getNickname() const;
  int getfd() const;
  void joinChannel(std::string channelName, Channel *channel);
  void leaveChannel(std::string channelName);
  std::map<std::string, Channel *> &getChannels();
};

#endif