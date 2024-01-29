#include "Privmsg.hpp"

#include <sys/select.h>

static std::string getTargetName(std::string param);

Privmsg::Privmsg(Request request, User *user)
    : server(Server::getInstance()),
      serverChannels(server.getChannelMap()),
      serverUsers(server.getUserMap()) {
  this->req = request;
  this->user = user;
  this->permission = DEFAULT;
  this->write_cnt = 0;
}

void Privmsg::execute() {
  if (validate()) {
    std::vector<std::string> params = req.parameter().getParameters();

    std::string targetName = getTargetName(params.at(0));
    if (targetName.at(0) == '#') {
      targetName = targetName.substr(1, std::string::npos);
      sendToChannel(targetName);
    } else {
      // Username이 포함된 경우 무시 (닉네임만 잘라서 진행)
      targetName = targetName.substr(0, targetName.find("!"));
      sendToUser(targetName);
    }
  }
  std::cout << msg << " " << write_cnt << std::endl;
  server.Send(msg, write_cnt, &fd_write);
}

void Privmsg::sendToUser(std::string &userName) {
  // 해당 유저 존재하지 않으면 오류 메시지 저장
  if (!serverUsers.exists(userName)) {
    msg = Response::error(ERR_NOSUCHNICK, *user, &fd_write);
    write_cnt = 1;
    return;
  }

  // 유저가 존재하는 경우 해당 유저에게 메시지 전송
  User &target = serverUsers.findUser(userName);
  std::string prefix = user->getNickname() + "!" + user->getUsername();

  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(),
                        req.parameter().getTrailer(), prefix);
  write_cnt = 1;
  std::cout << "target fd is " << target.getfd() << std::endl;
  FD_SET(target.getfd(), &fd_write);
}

void Privmsg::sendToChannel(std::string &channelName) {
  // 찾는 채널이 존재하지 않으면 오류 메시지 저장
  if (!serverChannels.exists(channelName)) {
    msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write);
    write_cnt = 1;
    return;
  }

  // 채널이 존재하는 경우 해당 채널의 모든 유저에게 전송
  Channel &channel = serverChannels.findChannel(channelName);
  std::vector<User *> channelUsers = channel.getUsers().findAllUsers();
  std::string prefix = user->getNickname() + "!" + user->getUsername();

  std::cout << "param size: " << req.parameter().getParameters().size()
            << std::endl;
  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(),
                        req.parameter().getTrailer(), prefix);
  write_cnt = channelUsers.size() - 1;
  for (size_t i = 0; i < channelUsers.size(); i++) {
    int fd = channelUsers.at(i)->getfd();
    if (fd != user->getfd()) FD_SET(fd, &fd_write);
  }
}

bool Privmsg::checkPermit() {
  return true;  // 메시지 권한은 항상 TRUE를 반환
}

static std::string getTargetName(std::string param) {
  char delim[2] = {'@', '%'};

  size_t pos = std::string::npos;
  for (int i = 0; i < 2; i++) {
    size_t tmp = param.find(delim[i]);
    if (tmp < pos) {
      pos = tmp;
    }
  }
  return param.substr(0, pos);
}

bool Privmsg::validate() {
  std::vector<std::string> params = req.parameter().getParameters();

  if (!checkPermit()) {
    msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write);
    write_cnt = 1;
    return false;
  } else if (params.size() < 1) {
    msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  return true;
}