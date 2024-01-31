#include "Pass.hpp"

Pass::Pass(Request request, User *user) : ICommand(request, user) {}

void Pass::execute() {
  Server &server = Server::getInstance();
  if (req.parameter().getParameters().size() != 1)  // 파라미터 갯수 안맞을때
  {
    this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    this->closeUser();
  } else if (server.auth(req.parameter().getParameters()[0]) ==
             0)  // 비밀번호 틀린 경우
  {
    this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    this->closeUser();
  } else  // 성공했을 경우
  {
    server.getcerti()[user->getfd()] = 1;
    std::vector<std::string> param;
    param.push_back("*");
    this->msg = Response::build("PRIVMSG", param, "PASSWORD IS CORRECT");
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    msg.clear();
    this->msg = Response::build("PRIVMSG", param, "PLZ INPUT USERNAME");
    send(user->getfd(), msg.c_str(), msg.size(), 0);
  }
}

bool Pass::checkPermit() { return (1); }

std::string Pass::getResponse() const { return msg; }

int Pass::getCount() const { return msg.size(); }

void Pass::closeUser()
{
    Server &server = Server::getInstance();
    close(user->getfd());
    int &totalusers = server.gettotalUsers();
    totalusers--;
    server.getUsedfd()[user->getfd()] = 0;
    server.getUserMap().deleteUser(user->getfd());
}