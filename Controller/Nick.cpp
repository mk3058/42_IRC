#include "Nick.hpp"

Nick::Nick(Request request, User *user) : ICommand(request, user) {}

void Nick::execute() {
  Server &server = Server::getInstance();
  if (req.parameter().getParameters().size() != 1)  // 파라미터 갯수 안맞을때
  {
    this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
    FD_SET(user->getfd(), &fd_write);
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
  } else if (checknick() == 1)  // 닉네임 인증 타이밍일때
  {
    if (server.getUserMap().exists(
            req.parameter().getParameters()[0]))  // 같은 닉네임 이미 있을때
    {
      this->msg = Response::error(ERR_NICKNAMEINUSE, *(this->user), &fd_write);
      FD_SET(user->getfd(), &fd_write);
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      std::vector<std::string> param;
      param.push_back("*");
      msg.clear();
      this->msg = Response::build("NOTICE", param, "already exist nickname");
      FD_SET(user->getfd(), &fd_write);
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      server.quitChUser(user->getfd());
      server.delUser(user->getfd());
    } else if (!checkname(
                   req.parameter().getParameters()[0]))  // 중복된 닉네임 없지만
                                                         // 설정 문법에 어긋날때
    {
      std::vector<std::string> param;
      param.push_back("*");
      param.push_back(req.parameter().getParameters()[0]);
      this->msg =
          Response::build(ERR_ERRONEUSNICKNAME, param, "Erroneous Nickname");
      FD_SET(user->getfd(), &fd_write);
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      ;
      server.quitChUser(user->getfd());
      server.delUser(user->getfd());
    } else  // 닉네임 설정이 잘 되었을
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
      Server::getInstance().bufferMessage(msg, 1, &fd_write);

      msg.clear();
      this->msg = Response::build("NOTICE", param, "PLZ INPUT USERNAME");
      FD_SET(newuser.getfd(), &fd_write);
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
    }
  } else if (checknick() == -1)  // 아직 비밀번호 인증단계 안 끝냈을때
  {
    this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
    FD_SET(user->getfd(), &fd_write);
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    server.getcerti()[user->getfd()]--;
    std::vector<std::string> param;
    param.push_back("*");
    msg.clear();
    this->msg = Response::build("NOTICE", param, "plz try valid password");
    FD_SET(user->getfd(), &fd_write);
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
  } else  // 이미 닉네임 인증 했을 때
  {
    std::vector<std::string> param;
    param.push_back("*");
    this->msg = Response::build("NOTICE", param, " is already seted nickname");
    FD_SET(user->getfd(), &fd_write);
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
  }
}

int Nick::checknick() {
  if (Server::getInstance().getcerti()[user->getfd()] <
      1)  // 아직 비밀번호 인증 안됐을때
    return -1;
  else if (Server::getInstance().getcerti()[user->getfd()] >
           1)  // 이미 닉네임 인증했을때
    return 0;
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
