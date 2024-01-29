#include "Invite.hpp"

Invite::Invite(Request request, User *user)
    : server(Server::getInstance()),
      serverChannels(server.getChannelMap()),
      serverUsers(server.getUserMap()) {
  this->req = request;
  this->user = user;
  this->write_cnt = 0;
  this->permission = DEFAULT;
}

void Invite::execute() {
  if (validate()) {
    std::vector<std::string> params = req.parameter().getParameters();
    Channel &channel = serverChannels.findChannel(params.at(1).substr(1));
    User &target = serverUsers.findUser(params.at(0));

    channel.getInvitedUsers().addUser(target.getfd(), target);
    if (channel.getBannedUsers().exists(target.getNickname())) {
      channel.getBannedUsers().deleteUser(target.getfd());
    }
    msg += Response::build(req.command().getCommand(),
                           req.parameter().getParameters(), "",
                           user->getNickname() + "!" + user->getUsername());
    write_cnt = 1;
    FD_SET(user->getfd(), &fd_write);
    noticeToChannel(params.at(1).substr(1));
    noticeToUser(params.at(0), params.at(1).substr(1));
  }
  server.Send(msg, write_cnt, &fd_write);
}

void Invite::noticeToChannel(std::string channelName) {
  std::vector<std::string> param;
  std::string noticeMsg;
  std::string trailer = user->getNickname() + " invited " +
                        req.parameter().getParameters().at(0) +
                        " to the channel #" + channelName;

  param.push_back("#" + channelName);
  noticeMsg = Response::build("NOTICE", param, trailer);

  Notice n;
  n.toChannel(channelName, noticeMsg);
}

void Invite::noticeToUser(std::string userName, std::string channelName) {
  std::vector<std::string> param;
  std::string noticeMsg;
  std::string trailer =
      user->getNickname() + " invited you to the channel #" + channelName;

  param.push_back(userName);
  noticeMsg = Response::build("NOTICE", param, trailer);

  Notice notice;
  notice.toUser(userName, noticeMsg);
}

bool Invite::checkPermit() {
  std::vector<std::string> params = req.parameter().getParameters();
  Channel &channel = serverChannels.findChannel(params.at(1).substr(1));

  if (channel.getMode() == INVITE_ONLY) {
    this->permission = USERMODE_SUPER;
  } else {
    this->permission = DEFAULT;
  }
  return (this->permission || channel.getUserPermits().at(user->getfd())) ==
         this->permission;
}

bool Invite::validate() {
  // 필요한 파라미터 모두 존재하는지 확인
  if (req.parameter().getParameters().size() < 2) {
    msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  // 대상 채널이 존재하는지 확인
  if (!serverChannels.exists(req.parameter().getParameters().at(1).substr(1))) {
    msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  // inviter가 해당 채널에 참여하고 있는지 확인
  Channel &channel = serverChannels.findChannel(
      req.parameter().getParameters().at(1).substr(1));
  if (!channel.getUsers().exists(user->getNickname())) {
    msg = Response::error(ERR_USERNOTINCHANNEL, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  // inviter가 초대 권한을 가지고 있는지 확인
  if (!checkPermit()) {
    msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  // invitee 가 존재하는지 확인
  if (!serverUsers.exists(req.parameter().getParameters().at(0))) {
    msg = Response::error(ERR_NOSUCHNICK, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  return true;
}