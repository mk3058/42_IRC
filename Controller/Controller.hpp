#ifndef Controller_HPP
#define Controller_HPP

#include <sys/select.h>

#include "Request.hpp"
#include "Server.hpp"

class User;

class Controller {
 private:
  Request &request;
  User *user;

 public:
  Controller(Request &req, User *user);
  void execute();
};

#endif
