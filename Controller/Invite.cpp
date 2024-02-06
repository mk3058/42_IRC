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

    sendRpl(params.at(1).substr(1));
    sendAck(params.at(0));
  } else {
    server.bufferMessage(msg, write_cnt, &fd_write);
  }
}

void Invite::sendRpl(std::string channelName) {
  std::vector<std::string> param = req.parameter().getParameters();
  std::string noticeMsg;

  param.insert(param.begin(), user->getNickname());
  noticeMsg += Response::build(RPL_INVITING, param, "");

  Notice n;
  n.toChannel(channelName, noticeMsg);
}

void Invite::sendAck(std::string userName) {
  std::vector<std::string> param;
  std::string noticeMsg;

  param.push_back(req.parameter().getParameters().at(0));
  noticeMsg += Response::build(req.command().getCommand(), param,
                               req.parameter().getParameters().at(1),
                               user->getNickname() + "!" + user->getUsername());

  Notice notice;
  notice.toUser(userName, noticeMsg);
}

bool Invite::checkPermit() {
  std::vector<std::string> params = req.parameter().getParameters();
  Channel &channel = serverChannels.findChannel(params.at(1).substr(1));

  if (channel.getMode() & INVITE_ONLY) {
    return channel.getUserPermits().at(user->getfd()) & USERMODE_SUPER;
  } else {
    return true;
  }
}

bool Invite::validate() {
  std::vector<std::string> params;

  // 필요한 파라미터 모두 존재하는지 확인
  if (req.parameter().getParameters().size() < 2) {
    msg = Response::build(ERR_NEEDMOREPARAMS, params, "More Parameter Needed!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // 대상 채널이 존재하는지 확인
  if (!serverChannels.exists(req.parameter().getParameters().at(1).substr(1))) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(1).substr(1));
    msg = Response::build(ERR_NOSUCHCHANNEL, params, "No such channel!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // inviter가 해당 채널에 참여하고 있는지 확인
  Channel &channel = serverChannels.findChannel(
      req.parameter().getParameters().at(1).substr(1));
  if (!channel.getUsers().exists(user->getNickname())) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(1));
    msg = Response::build(ERR_USERNOTINCHANNEL, params,
                          "Inviter is not on the channel!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // inviter가 초대 권한을 가지고 있는지 확인
  if (!checkPermit()) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(1));
    msg = Response::build(ERR_CHANOPRIVSNEEDED, params,
                          "You're not channel operator!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // invitee 가 존재하는지 확인
  if (!serverUsers.exists(req.parameter().getParameters().at(0))) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(0));
    msg = Response::build(ERR_NOSUCHNICK, params, "No such nick/channel!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // invitee 가 이미 해당 채널에 참여하고 있는지 확인
  if (channel.getUsers().exists(req.parameter().getParameters().at(0))) {
    params.push_back(req.parameter().getParameters().at(0));
    params.push_back(req.parameter().getParameters().at(1));
    msg = Response::build(ERR_USERONCHANNEL, params,
                          "User is already in the channel!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // invitee 가 이미 해당 채널에 초대되어 있는지 확인
  if (channel.getInvitedUsers().exists(req.parameter().getParameters().at(0))) {
    params.push_back(req.parameter().getParameters().at(0));
    params.push_back(req.parameter().getParameters().at(1));
    msg =
        Response::build(ERR_USERONCHANNEL, params, "User is already invited!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  return true;
}