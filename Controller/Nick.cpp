#include "Nick.hpp"

Nick::Nick(Request request, User *user) : ICommand(request, user) {}

void Nick::execute() {
  Server &server = Server::getInstance();
  if (req.parameter().getParameters().size() != 1)  // 파라미터 갯수 안맞을때
  {
    this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    this->closeUser();
  } else if (checknick() == 1)  // 닉네임 인증 타이밍일때
  {
    if (server.getUserMap().exists(
            req.parameter().getParameters()[0]))  // 같은 닉네임 이미 있을때
    {
      this->msg = Response::error(ERR_NICKNAMEINUSE, *(this->user), &fd_write);
      send(user->getfd(), msg.c_str(), msg.size(), 0);
      std::vector<std::string> param;
      param.push_back("*");
      msg.clear();
      this->msg = Response::build("PRIVMSG", param, "already exist nickname");
      send(user->getfd(), msg.c_str(), msg.size(), 0);
      this->closeUser();
    } else if (!checkname(req.parameter().getParameters()[0])) //중복된 닉네임 없지만 설정 문법에 어긋날때
    {
      std::vector<std::string> param;
      param.push_back("*");
      param.push_back(req.parameter().getParameters()[0]);
      this->msg =
            Response::build(ERR_ERRONEUSNICKNAME, param, "Erroneous Nickname");
      send(user->getfd(), msg.c_str(), msg.size(), 0);
      this->closeUser();
      } else  // 닉네임 설정이 잘 되었을
      {
        User newuser = server.getUserMap().findUser(user->getfd());
        server.getUserMap().deleteUser(user->getfd());
        newuser.setNickname(req.parameter().getParameters()[0]);
        server.getUserMap().addUser(newuser.getfd(), newuser);
        server.getcerti()[newuser.getfd()]++;
        std::vector<std::string> param;
        param.push_back("*");
        this->msg = Response::build("PRIVMSG", param, "NICKNAME IS SET");
        send(newuser.getfd(), msg.c_str(), msg.size(), 0);
        msg.clear();
        this->msg = Response::build("PRIVMSG", param, "PLZ INPUT USERNAME");
        send(newuser.getfd(), msg.c_str(), msg.size(), 0);
      }
    }
  else if (checknick() == -1)  // 아직 비밀번호 인증단계 안 끝냈을때
  {
    this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    server.getcerti()[user->getfd()]--;
    std::vector<std::string> param;
    param.push_back("*");
    msg.clear();
    this->msg = Response::build("PRIVMSG", param, "plz try valid password");
    send(user->getfd(), msg.c_str(), msg.size(), 0);
    this->closeUser();
  } else  // 이미 닉네임 인증 했을 때
  {
    std::vector<std::string> param;
    param.push_back("*");
    this->msg = Response::build("PRIVMSG", param, " is already seted nickname");
    send(user->getfd(), msg.c_str(), msg.size(), 0);
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

void Nick::closeUser()
{
  Server &server = Server::getInstance();
  close(user->getfd());
  server.getUsedfd()[user->getfd()] = 0;
  server.getUserMap().deleteUser(user->getfd());
  server.getcerti()[user->getfd()] = 0;
}