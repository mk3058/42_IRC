#include "Kick.hpp"

Kick::Kick(Request req, User *user) : ICommand(req, user) {
  param = true;
  this->channelMap = &Server::getInstance().getChannelMap();
  if (req.parameter().getParameters().size() < 2)
    param = false;
  else if (channelMap->exists(req.parameter().getParameters()[0].substr(1))) {
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
  std::string trailer = req.parameter().isTrailerExists()
                            ? req.parameter().getTrailer()
                            : "No reason";
  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(), trailer,
                        user->getNickname() + "!" + user->getUsername());
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
  std::vector<std::string> params;
  int fd = user->getfd();
  FD_SET(fd, &fd_write);

  // 파라미터 부족
  if (param == false) {
    msg = Response::build(ERR_NEEDMOREPARAMS, params, "More Parameter Needed!");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 채널이 없음
  if (!channelMap->exists(req.parameter().getParameters()[0].substr(1))) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters()[0]);
    msg = Response::build(ERR_NOSUCHCHANNEL, params, "No such channel!");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 권한확인이 먼저
  int upermission = channel->getUserPermits()[user->getfd()];
  if (!(upermission & USERMODE_SUPER)) {
    params.push_back(user->getNickname());
    params.push_back("#" + channel->getName());
    msg = Response::build(ERR_CHANOPRIVSNEEDED, params,
                          "You're not channel operator!");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 유저네임이 잘못됨
  if (!channel->getUsers().exists(req.parameter().getParameters()[1])) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters()[1]);
    msg = Response::build(ERR_NOSUCHNICK, params, "No such nick/channel!");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  return (true);
}
