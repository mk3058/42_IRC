#include "UserCmd.hpp"
#include <sstream>

UserCmd::UserCmd(Request request, User *user) : ICommand(request, user) {}

void UserCmd::execute() {
  Server &server = Server::getInstance();
  if (!req.parameter().getParameters().size())
  {
    msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
    FD_SET(user->getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);
  }
  else if (!checkPermit())  // 아직 앞 단계 인증이 안됐을때
  {
    this->msg = Response::error(ERR_NOTREGISTERED, *(this->user), &fd_write);
    FD_SET(user->getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);
    std::vector<std::string> param;
    param.push_back("*");
    msg.clear();
    this->msg = Response::build("NOTICE", param, "Not yet resistered");
    FD_SET(user->getfd(), &fd_write);
    server.bufferMessage(msg, 1, &fd_write);
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
  } else  // 인증 단계일때
  {
    if (!checkname(req.parameter().getParameters()[0]))  // 유저 네임 이상할때
    {
      this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
      FD_SET(user->getfd(), &fd_write);
      server.bufferMessage(msg, 1, &fd_write);
      server.quitChUser(user->getfd());
      server.delUser(user->getfd());
    } else  // 인증 성공 했을 때
    {
      server.getUserMap().setUsername(user->getfd(),
                                      req.parameter().getParameters()[0]);
      this->welcome();
      this->yourHost();
      server.getcerti()[user->getfd()] = 3;
      std::cout << user->getfd() << "(Client) is resistered" << std::endl;
    }
  }
}

bool UserCmd::checkPermit() {
  if (Server::getInstance().getcerti()[user->getfd()] < 2)
    return 0;  // 비밀번호 및 닉네임 설정이 안됐을때
  return 1;    // 유저네임 인증해야 할때
}

int UserCmd::getCount() const { return msg.size(); }

std::string UserCmd::getResponse() const { return msg; }

bool UserCmd::checkname(std::string name) {
  int len = name.size();
  if (len <= 1 || len >= 10) return false;
  for (int i = 0; i < len; i++) {
    if (!std::isalnum(name[i])) return false;
  }
  return true;
}

void UserCmd::welcome()
{
  Server &server = Server::getInstance();
  std::vector<std::string> param;
  param.push_back(user->getNickname());
  sockaddr_in addr;
  socklen_t addr_len = sizeof(server.getstruct());
  getsockname(server.getsockfd(), (struct sockaddr *)&addr, &addr_len);
  std::ostringstream oss;
  oss << "Welcome to the ircserv.com Network, " <<\
  "" << user->getNickname() << "[!" << user->getUsername() << "@" << inet_ntoa(addr.sin_addr) \
  << "]";
  std::string trailer = oss.str();
  msg = Response::build(RPL_WELCOME, param, trailer);
  FD_SET(user->getfd(), &fd_write);
  server.bufferMessage(msg, 1, &fd_write);
  msg.clear();
}

void UserCmd::yourHost()
{
  std::string trailer;
  Server &server = Server::getInstance();
  std::vector<std::string> param;
  param.push_back(user->getNickname());
  std::ostringstream oss;
  oss << "Your host is ircserv.com, running version "\
  << "1.4.5";
  trailer = oss.str();
  msg = Response::build(RPL_YOURHOST, param, trailer);
  FD_SET(user->getfd(), &fd_write);
  server.bufferMessage(msg, 1, &fd_write);
  msg.clear();
}