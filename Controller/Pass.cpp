#include "Pass.hpp"

Pass::Pass(Request request, User *user) : ICommand(request, user) {}

void Pass::execute() {
  Server &server = Server::getInstance();
  if (req.parameter().getParameters().size() != 1 || 
    server.auth(req.parameter().getParameters()[0]) == 0)  // 파라미터 갯수 안맞을때
  {
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
  }
  else  // 성공했을 경우
  {
    server.getcerti()[user->getfd()] = 1;
    std::vector<std::string> param;
    param.push_back("*");
    this->msg = Response::build("NOTICE", param, "PASSWORD IS CORRECT");
    FD_SET(user->getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);
    msg.clear();
    this->msg = Response::build("NOTICE", param, "PLZ INPUT NICKNAME");
    FD_SET(user->getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);
  }
}

bool Pass::checkPermit() { return (1); }

std::string Pass::getResponse() const { return msg; }

int Pass::getCount() const { return msg.size(); }
