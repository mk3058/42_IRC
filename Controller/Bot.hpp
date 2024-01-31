#ifndef BOT_HPP
#define BOT_HPP

#include "Request.hpp"

class Bot {
 private:
  std::vector<std::string> breakfast;
  std::vector<std::string> lunch;
  std::vector<std::string> dinner;
  int user_fd;
  Request req;
  std::string nickname;

 public:
  Bot(Request req, int fd, std::string nickname);
  void execute();
  void menuInit();
  std::string findmenu(std::string str);
};

#endif