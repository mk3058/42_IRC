#include "Join.hpp"

Join::Join(Request req, User *user) : ICommand(req, user) {
  this->channelMap = &Server::getInstance().getChannelMap();
  if (!channelMap->exists(req.parameter().getParameters()[0].substr(1)))
    channelMap->addChannel(
        Channel(req.parameter().getParameters()[0].substr(1)));
  this->channel =
      &(channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
  this->permission = this->channel->getMode();
}

void Join::execute() {
  if (!checkPermit(user->getfd())) return;
  // 유저의 채널맵 업데이트
  user->joinChannel(req.parameter().getParameters()[0].substr(1), channel);
  // 채널에도 유저추가
  channel->addUser(*user);
  // 유저 들어왔다는 메시지 전송
  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(),
                        "user " + user->getNickname() + " join this channel",
                        user->getNickname() + "!" + user->getUsername() + "@" DEFAULT_PREFIX);
  write_cnt = channel->getUsers().getSize();
  for (int i = 0; i < write_cnt; ++i)
    FD_SET(channel->getUsers().findAllUsers()[i]->getfd(), &fd_write);
  Server::getInstance().bufferMessage(msg, write_cnt, &fd_write);
}

bool Join::checkPermit(int fd) {
  FD_SET(fd, &fd_write);
  if (channel->getUserLimit() == channel->getUsers().getSize()) {
    msg =
        Response::error(ERR_CHANNELISFULL, *user, &fd_write, "channel is full");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (channel->getUsers().exists(fd)) return (false);
  if (channel->getBannedUsers().exists(fd)) {
    msg = Response::error(ERR_BANNEDFROMCHAN, *user, &fd_write,
                          "you are banned from channel");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (this->permission & INVITE_ONLY &&
      !channel->getInvitedUsers().exists(user->getNickname())) {
    msg = Response::error(ERR_INVITEONLYCHAN, *user, &fd_write,
                          "invite only channel");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (this->permission & KEY_REQURIE) {
    if (req.parameter().getParameters().size() < 2)
    {
      msg =
          Response::error(ERR_BADCHANNELKEY, *user, &fd_write, "you need key");
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      return (false);
    }
    if (channel->getPassword() != req.parameter().getParameters()[1])
    {
      msg = Response::error(ERR_BADCHANNELKEY, *user, &fd_write,
                            "wrong key, try again");
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
      return (false);
    }
  }
  return (true);
}
