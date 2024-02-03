#include "Nick.hpp"

Nick::Nick(Request request, User *user) : ICommand(request, user) {}

void Nick::execute() {
  Server &server = Server::getInstance();
  if (req.parameter().getParameters().size() != 1 || checknick() == -1 ||\
      server.getUserMap().exists(req.parameter().getParameters()[0]) || \
      !checkname(req.parameter().getParameters()[0])) // 파라미터 제대로 없거나 인증단계 아닐때
  {
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
    return ;
  }
  else  // 닉네임 설정이 잘 되었을
  {
    User newuser = server.getUserMap().findUser(user->getfd());
    server.getUserMap().deleteUser(user->getfd());
    newuser.setNickname(req.parameter().getParameters()[0]);
    server.getUserMap().addUser(newuser.getfd(), newuser);
    server.getcerti()[newuser.getfd()]++;
    std::vector<std::string> param;
    param.push_back("*");
    this->msg = Response::build("NOTICE", param, "NICKNAME IS SET");
    FD_SET(newuser.getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);

    msg.clear();
    this->msg = Response::build("NOTICE", param, "PLZ INPUT USERNAME");
    FD_SET(newuser.getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);
  }
}

int Nick::checknick() {
  if (Server::getInstance().getcerti()[user->getfd()] <
      1)  // 아직 비밀번호 인증 안됐을때
    return -1;
  return 1;  // 닉네임 인증 해야할 때
}

std::string Nick::getResponse() const { return (msg); }

int Nick::getCount() const { return (msg.size()); }

bool Nick::checkname(std::string name) {
  int len = name.size();
  if (len <= 1 || len >= 10) return false;
  for (int i = 0; i < len; i++) {
    if (!std::isalnum(name[i])) return false;
  }
  return true;
}
