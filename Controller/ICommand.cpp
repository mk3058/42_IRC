#include "ICommand.hpp"

ICommand::ICommand() : user(NULL) { FD_ZERO(&fd_write); }

ICommand::ICommand(Request request, User *user)
    : req(request), user(user) {
  FD_ZERO(&fd_write);
}

bool ICommand::checkPermit() {
  if (permission == USERMODE_SUPER) return (true);
  return (false);
}

std::string ICommand::getResponse() const { return this->msg; }

int ICommand::getCount() const { return this->write_cnt; }