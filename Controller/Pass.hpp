#ifndef PASS_HPP
#define PASS_HPP

#include "ICommand.hpp"

class Pass : public ICommand {
 private:
 public:
  Pass(Request request, User *user);

  void execute();
  bool checkPermit();
  std::string getResponse() const;
  int getCount() const;
};

#endif