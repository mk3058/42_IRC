#include "Kick.hpp"

Kick::Kick(Request req, User *user) : ICommand(req, user) {
  param = true;
  this->channelMap = &Server::getInstance().getChannelMap();
  if (!req.parameter().getParameters().size()) param = false;
  else if (channelMap->exists(req.parameter().getParameters()[0].substr(1)))
  {
    this->channel = &(
        channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
    this->permission = channel->getMode();
  }
}

void Kick::execute() {
  if (!checkPermit()) return;
  // 유저의 채널정보 업데이트
  std::string userName = req.parameter().getParameters()[1];
  User *targetUser = &channel->getUsers().findUser(userName);
  targetUser->leaveChannel(channel->getName());
  //  내보내졌다는 메시지 보내기
  msg = Response::build(
      req.command().getCommand(), req.parameter().getParameters(),
      "user " + targetUser->getNickname() + " Kick this channel",
      user->getNickname() + "!" + user->getUsername() + "@" DEFAULT_PREFIX);
  write_cnt = channel->getUsers().getSize();
  for (int i = 0; i < write_cnt; ++i)
    FD_SET(channel->getUsers().findAllUsers()[i]->getfd(), &fd_write);
  Server::getInstance().bufferMessage(msg, write_cnt, &fd_write);
  // 유저 지우기
  channel->getBannedUsers().addUser(targetUser->getfd(), *targetUser);
  channel->deleteUser(*targetUser);
  // 마지막 유저였으면 채널도 삭제
  if (!channel->getUsers().getSize())
    channelMap->deleteChannel(channel->getName());
}

bool Kick::checkPermit() {
  int fd = user->getfd();
  FD_SET(fd, &fd_write);
  // 채널명이 잘못됨
  if (param == false)
  {
    msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write, "parameter error(kick)");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (!channelMap->exists(req.parameter().getParameters()[0].substr(1))) {
    msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write,
                          "wrong channel name");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 권한확인이 먼저
  int upermission = channel->getUserPermits()[user->getfd()];
  if (!(upermission & USERMODE_SUPER)) {
    msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write,
                          "you are not operator");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 유저네임이 안들어옴
  if (req.parameter().getParameters().size() < 2) {
    msg =
        Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write, "need user name");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 유저네임이 잘못됨
  if (!channel->getUsers().exists(req.parameter().getParameters()[1])) {
    msg = Response::error(ERR_USERNOTINCHANNEL, *user, &fd_write,
                          "user not in channel");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  return (true);
}
