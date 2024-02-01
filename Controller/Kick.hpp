#ifndef Kick_HPP
#define Kick_HPP

#include "ICommand.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "StatusCode.hpp"

class Kick : public ICommand {
 private:
  ChannelMap *channelMap;
  Channel *channel;
  bool checkPermit();
  bool param;

 public:
  void execute();
  Kick(Request req, User *user);
};

#endif