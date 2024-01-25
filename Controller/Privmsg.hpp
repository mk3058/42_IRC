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
  static Server &server;
  static ChannelMap &serverChannels;
  static UserMap &serverUsers;

 public:
  Privmsg(Request request, User *user);
  virtual void execute();
  virtual bool checkPermit();
};

#endif