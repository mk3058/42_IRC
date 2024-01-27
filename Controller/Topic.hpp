#ifndef TOPIC_HPP
#define TOPIC_HPP

#include <vector>

#include "ICommand.hpp"
#include "Server.hpp"
#include "StatusCode.hpp"

class Topic : public ICommand {
 private:
  bool validate();
  void changeTopic();
  void showTopic();
  Server &server;
  ChannelMap &serverChannels;
  UserMap &serverUsers;

 public:
  Topic(Request request, User *user);
  virtual void execute();
  virtual bool checkPermit();
};

#endif