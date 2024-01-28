#ifndef INVITE_HPP
#define INVITE_HPP

#include "ICommand.hpp"

class Invite : public ICommand {
 private:
  Server &server;
  ChannelMap &serverChannels;
  UserMap &serverUsers;
  bool validate();

 public:
  Invite(Request request, User *user);
  virtual void execute();
  virtual bool checkPermit();
};

#endif