#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <iostream>

#include "Channel.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "StatusCode.hpp"

class ICommand {
 protected:
  Request req;
  User *user;
  int permission;
  int write_cnt;
  fd_set fd_write;
  std::string msg;
  virtual void execute() = 0;
  virtual bool checkPermit();

 public:
  ICommand();
  ICommand(Request request, User *user);
  virtual std::string getResponse() const;
  virtual int getCount() const;
};

#endif