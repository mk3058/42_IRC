#include "Part.hpp"

Part::Part(Request req, User *user) : ICommand(req, user) {
  param = true;
  this->channelMap = &Server::getInstance().getChannelMap();
  if (!req.parameter().getParameters().size()) param = false;
  else if (channelMap->exists(req.parameter().getParameters()[0].substr(1))) {
    this->channel = &(
        channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
    this->permission = channel->getMode();
  }
}

void Part::execute() {
  if (!checkPermit()) return;
  // 메시지 모든 채널에 속한 유저에게 전송 자신에게도 전송해야해서 메시지부터
  // 보냄
  std::vector<std::string> emptyVec;
  msg = Response::build(
      req.command().getCommand(), emptyVec, req.parameter().getParameters()[0],
      user->getNickname() + "!" + user->getUsername() + "@" DEFAULT_PREFIX);
  write_cnt = channel->getUsers().getSize();
  for (int i = 0; i < write_cnt; ++i)
    FD_SET(channel->getUsers().findAllUsers()[i]->getfd(), &fd_write);
  Server::getInstance().bufferMessage(msg, write_cnt, &fd_write);
  // 채널정보 업데이트
  std::string channelName = req.parameter().getParameters()[0].substr(1);
  user->leaveChannel(channelName);
  // 유저 지우기
  channel->deleteUser(*user);
  // 마지막 유저였으면 채널도 삭제
  if (!channel->getUsers().getSize())
    channelMap->deleteChannel(channel->getName());
}

bool Part::checkPermit() {
  int fd = user->getfd();
  FD_SET(fd, &fd_write);
  if (param == false)
  {
    msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write, "parameter error(part)");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (!channelMap->exists(req.parameter().getParameters()[0].substr(1))) {
    msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write,
                          "wrong channel name");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (!channel->getUsers().exists(user->getfd())) {
    msg = Response::error(ERR_NOTONCHANNEL, *user, &fd_write,
                          "you are not on channel");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  return (true);
}
