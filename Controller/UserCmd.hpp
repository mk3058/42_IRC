#ifndef USERCMD_HPP
#define USERCMD_HPP

#include "ICommand.hpp"

class UserCmd : public ICommand {
 private:
 public:
  UserCmd(Request request, User *user);

  void execute();
  bool checkPermit();
  int getCount() const;
  std::string getResponse() const;
  bool checkname(std::string name);
  void welcome();
  void yourHost();
  void creatTime();
  void myInfo();
};

#endif