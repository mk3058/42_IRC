#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "ICommand.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "StatusCode.hpp"
#include "User.hpp"

class Privmsg : public ICommand {
 private:
  void sendToChannel(std::string &channelName);
  void sendToUser(std::string &userName);
  bool validate();
  Server &server;
  ChannelMap &serverChannels;
  UserMap &serverUsers;

 public:
  Privmsg(Request request, User *user);
  virtual void execute();
  virtual bool checkPermit();
  std::string bot();
};

#endif