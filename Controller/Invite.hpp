#ifndef INVITE_HPP
#define INVITE_HPP

#include "ICommand.hpp"
#include "Notice.hpp"

class Invite : public ICommand {
 private:
  Server &server;
  ChannelMap &serverChannels;
  UserMap &serverUsers;
  bool validate();
  void sendRpl(std::string channelName);
  void sendAck(std::string userName);

 public:
  Invite(Request request, User *user);
  virtual void execute();
  virtual bool checkPermit();
};

#endif