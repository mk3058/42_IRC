#include "UserCmd.hpp"
#include <sstream>

UserCmd::UserCmd(Request request, User *user) : ICommand(request, user) {}

void UserCmd::execute() {
  Server &server = Server::getInstance();
  if (!req.parameter().getParameters().size() || !checkPermit() \
    || !checkname(req.parameter().getParameters()[0]))
  {
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
  }
  else  // 인증 성공 했을 때
    {
      server.getUserMap().setUsername(user->getfd(),
                                      req.parameter().getParameters()[0]);
      this->welcome();
      this->yourHost();
      this->creatTime();
      this->myInfo();
      server.getcerti()[user->getfd()] = 3;
      std::cout << user->getfd() << "(Client) is resistered" << std::endl;
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
  Server &server = Server::getInstance();
  std::vector<std::string> param;
  param.push_back(user->getNickname());
  std::ostringstream oss;
  oss << "Your host is ircserv.com, running version "\
  << "1.4.5";
  std::string trailer = oss.str();
  msg = Response::build(RPL_YOURHOST, param, trailer);
  FD_SET(user->getfd(), &fd_write);
  server.bufferMessage(msg, 1, &fd_write);
  msg.clear();
}

void UserCmd::creatTime()
{
  Server &server = Server::getInstance();
  std::ostringstream oss;
  time_t t = server.getTime();
  struct tm* timeinfo = localtime(&t);
  std::vector<std::string> param;
  param.push_back(user->getNickname());

  char buf[50];
  memset(buf, 0, sizeof(buf));
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
  oss << "This server was created " << buf;
  std::string trailer = oss.str();
  msg = Response::build(RPL_CREATED, param, trailer);
  FD_SET(user->getfd(), &fd_write);
  server.bufferMessage(msg, 1, &fd_write);
  msg.clear();
}

void UserCmd::myInfo()
{
  Server &server = Server::getInstance();
  std::vector<std::string> param;
    
  param.push_back(user->getNickname());
  param.push_back("ircserv.com");
  param.push_back("42irc-1");
  param.push_back("-");
  param.push_back("iotlk");
  param.push_back("lk");

  msg = Response::build(RPL_MYINFO, param);

  FD_SET(user->getfd(), &fd_write);
  server.bufferMessage(msg, 1, &fd_write);
  msg.clear(); 
}