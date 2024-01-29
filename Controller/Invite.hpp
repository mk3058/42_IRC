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
  void noticeToChannel(std::string channelName);
  void noticeToUser(std::string userName, std::string channelName);

 public:
  Invite(Request request, User *user);
  virtual void execute();
  virtual bool checkPermit();
};

#endif