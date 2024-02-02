#include "Part.hpp"

Part::Part(Request req, User *user) : ICommand(req, user) {
  param = true;
  this->channelMap = &Server::getInstance().getChannelMap();
  if (!req.parameter().getParameters().size())
    param = false;
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
  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(),
                        req.parameter().getTrailer(),
                        user->getNickname() + "!" + user->getUsername());
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
  // 요청 유저가 해당 채널에 참여중이지 않음
  if (!channel->getUsers().exists(user->getfd())) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters()[0]);
    msg = Response::build(ERR_USERNOTINCHANNEL, params,
                          "You're not on the channel!");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  return (true);
}
