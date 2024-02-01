#ifndef MODE_HPP
#define MODE_HPP

#include <sstream>

#include "ICommand.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "StatusCode.hpp"

class Mode : public ICommand {
 private:
  ChannelMap *channelMap;
  Channel *channel;
  bool checkPermit();
  bool checkParam(unsigned long cnt);
  bool kmode(std::string type);
  bool imode(std::string type);
  bool lmode(std::string type);
  bool omode(std::string type);
  bool tmode(std::string type);
  bool param;

 public:
  void execute();
  Mode(Request req, User *user);
};

#endif