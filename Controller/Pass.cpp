#include "Pass.hpp"

Pass::Pass(Request request, User *user) : ICommand(request, user) {}

void Pass::execute() {
  Server &server = Server::getInstance();
  if (req.parameter().getParameters().size() != 1)  // 파라미터 갯수 안맞을때
  {
    this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
    send(user->getfd(), msg.c_str(), msg.size(), 0);
  } else if (server.auth(req.parameter().getParameters()[0]) ==
             0)  // 비밀번호 틀린 경우
  {
    this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    close(user->getfd());
    server.getUserMap().deleteUser(user->getfd());
    server.getUsedfd()[user->getfd()] = 0;
    std::cout << user->getfd() << " is disconnected by attempt three times."
              << std::endl;

  } else  // 성공했을 경우
  {
    server.getcerti()[user->getfd()] = 1;
    std::vector<std::string> param;
    param.push_back("*");
    this->msg = Response::build("PRIVMSG", param, "plz input nickname");
    send(user->getfd(), msg.c_str(), msg.size(), 0);
  }
}

bool Pass::checkPermit() { return (1); }

std::string Pass::getResponse() const { return msg; }

int Pass::getCount() const { return msg.size(); }