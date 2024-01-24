#include "ICommand.hpp"

ICommand::ICommand() : fd_write(NULL), user(NULL) {}

ICommand::ICommand(Request request, User *user, fd_set *fd_write)
    : req(request), user(user), fd_write(fd_write) {}

bool ICommand::checkPermit() {
  if (permission == USERMODE_SUPER) return (true);
  return (false);
}

std::string ICommand::getResponse() const { return this->msg; }

int ICommand::getCount() const { return this->write_cnt; }