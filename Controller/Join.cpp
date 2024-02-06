#include "Join.hpp"

Join::Join(Request req, User *user) : ICommand(req, user) {
  this->channelMap = &Server::getInstance().getChannelMap();
  param = true;
  if (!req.parameter().getParameters().size())
    param = false;
  else {
    if (!channelMap->exists(req.parameter().getParameters()[0].substr(1)))
      channelMap->addChannel(
          Channel(req.parameter().getParameters()[0].substr(1)));
    this->channel = &(
        channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
    this->permission = this->channel->getMode();
  }
}

void Join::execute() {
  if (!checkPermit(user->getfd())) return;
  // 유저의 채널맵 업데이트
  user->joinChannel(req.parameter().getParameters()[0].substr(1), channel);
  // 채널에도 유저추가
  channel->addUser(*user);
  // 초대리스트에선 뺴기
  if (channel->getInvitedUsers().exists(user->getfd()))
    channel->getInvitedUsers().deleteUser(user->getfd());

  // 채널에 속한 모든 유저에게 메시지 전송위해서 플래그 설정
  write_cnt = channel->getUsers().getSize();
  for (int i = 0; i < write_cnt; ++i)
    FD_SET(channel->getUsers().findAllUsers()[i]->getfd(), &fd_write);

  // 유저 들어왔다는 메시지 전송
  msg = Response::build(
      req.command().getCommand(), req.parameter().getParameters(),
      "user " + user->getNickname() + " join this channel",
      user->getNickname() + "!" + user->getUsername() + "@" DEFAULT_PREFIX);
  Server::getInstance().bufferMessage(msg, write_cnt, &fd_write);

  // 들어온 유저한테 보내줘야할 메시지 셋팅
  FD_ZERO(&fd_write);
  FD_SET(user->getfd(), &fd_write);
  std::vector<std::string> tmp;
  const Parameter &params = req.parameter();
  tmp.push_back(user->getNickname());
  tmp.push_back(params.getParameters().at(0));
  // 토픽있으면 토픽
  if (!channel->getTopic().empty()) {
    msg = Response::build(RPL_TOPIC, tmp, channel->getTopic());
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
  }
  // 유저리스트 만들기
  std::string usersName;
  
  for (int i = 0; i < write_cnt; ++i) {
    User *tmp_usr = channel->getUsers().findAllUsers()[i];
    if (channel->getUserPermits()[tmp_usr->getfd()] & USERMODE_SUPER)
      usersName += " @";
    else
      usersName += " ";
    usersName += tmp_usr->getNickname();
  }
  // 유저리스트보내기
  std::vector<std::string> tmp2;
  tmp2.push_back(user->getNickname());
  tmp2.push_back("=");
  tmp2.push_back(params.getParameters().at(0));
  msg = Response::build(RPL_NAMREPLY, tmp2, usersName);
  Server::getInstance().bufferMessage(msg, 1, &fd_write);
  msg = Response::build(RPL_ENDOFNAMES, tmp, ":End of /NAMES list");
  Server::getInstance().bufferMessage(msg, 1, &fd_write);


}

bool Join::checkPermit(int fd) {
  std::vector<std::string> params;
  FD_SET(fd, &fd_write);
  if (this->param == false) {
    msg = Response::build(ERR_NEEDMOREPARAMS, params, "More Parameter Needed!");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (channel->getUserLimit() == channel->getUsers().getSize()) {
    params.push_back(user->getNickname());
    params.push_back("#" + channel->getName());
    msg = Response::error(ERR_CHANNELISFULL, *user, &fd_write,
                          ":Cannot join channel (+l)");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (channel->getUsers().exists(fd)) return (false);
  if (channel->getBannedUsers().exists(fd)) {
    params.push_back(user->getNickname());
    params.push_back("#" + channel->getName());
    msg = Response::error(ERR_BANNEDFROMCHAN, *user, &fd_write,
                          ":Cannot join channel (+b)");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (this->permission & INVITE_ONLY &&
      !channel->getInvitedUsers().exists(user->getNickname())) {
    params.push_back(user->getNickname());
    params.push_back("#" + channel->getName());
    msg = Response::error(ERR_INVITEONLYCHAN, *user, &fd_write,
                          ":Cannot join channel (+i)");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (this->permission & KEY_REQURIE) {
    if (req.parameter().getParameters().size() < 2) {
      params.push_back(user->getNickname());
      params.push_back("#" + channel->getName());
      msg = Response::build(ERR_BADCHANNELKEY, params,
                            ":Cannot join channel (+k)");
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      return (false);
    }
    if (channel->getPassword() != req.parameter().getParameters()[1]) {
      params.push_back(user->getNickname());
      params.push_back("#" + channel->getName());
      msg = Response::build(ERR_BADCHANNELKEY, params,
                            ":Cannot join channel (+k)");
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      return (false);
    }
  }
  return (true);
}
