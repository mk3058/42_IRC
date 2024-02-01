#include "Topic.hpp"

Topic::Topic(Request request, User *user)
    : server(Server::getInstance()),
      serverChannels(server.getChannelMap()),
      serverUsers(server.getUserMap()) {
  this->req = request;
  this->user = user;
  this->write_cnt = 0;
}

void Topic::execute() {
  if (validate()) {
    if (req.parameter().isTrailerExists()) {
      changeTopic();
    } else {
      showTopic();
    }
  }
  server.bufferMessage(msg, write_cnt, &fd_write);
}

void Topic::changeTopic() {
  const Parameter &params = req.parameter();
  Channel &channel =
      serverChannels.findChannel(params.getParameters().at(0).substr(1));
  std::vector<User *> channelUsers = channel.getUsers().findAllUsers();

  channel.setTopic(params.getTrailer());

  std::string prefix = user->getNickname() + "!" + user->getUsername();
  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(),
                        req.parameter().getTrailer(), prefix);
  write_cnt = channelUsers.size();
  for (int i = 0; i < write_cnt; i++) {
    FD_SET(channelUsers.at(i)->getfd(), &fd_write);
  }
}

void Topic::showTopic() {
  const Parameter &params = req.parameter();
  Channel &channel =
      serverChannels.findChannel(params.getParameters().at(0).substr(1));
  std::vector<std::string> tmp;
  tmp.push_back(user->getNickname());
  tmp.push_back(params.getParameters().at(0));

  if (channel.getTopic().empty()) {
    msg = Response::build(RPL_NOTOPIC, tmp, "No topic is set");
  } else {
    msg = Response::build(RPL_TOPIC, tmp, channel.getTopic());
  }
  write_cnt = 1;
  FD_SET(user->getfd(), &fd_write);
}

bool Topic::checkPermit() {
  std::vector<std::string> params = req.parameter().getParameters();
  Channel &channel = serverChannels.findChannel(params.at(0).substr(1));

  if (!req.parameter().isTrailerExists()) {
    return true;
  } else {
    return (!(channel.getMode() & TOPIC_ONLY_OPERATOR) ||
            channel.getUserPermits().at(user->getfd()) & USERMODE_SUPER);
  }
}

bool Topic::validate() {
  std::vector<std::string> params;

  // 파라미터 개수 확인
  if (req.parameter().getParameters().size() < 1) {
    msg = Response::build(ERR_NEEDMOREPARAMS, params, "More Parameter Needed!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // 해당 채널이 존재하는지 확인
  if (!serverChannels.exists(req.parameter().getParameters().at(0).substr(1))) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(0));
    msg = Response::build(ERR_NOSUCHCHANNEL, params, "No such channel!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // 권한 확인
  if (!checkPermit()) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(0));
    msg = Response::build(ERR_CHANOPRIVSNEEDED, params,
                          "You're not channel operator!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  // 요청 유저가 채널에 참여하고 있는지 확인
  Channel &channel = serverChannels.findChannel(
      req.parameter().getParameters().at(0).substr(1));
  if (!channel.getUsers().exists(user->getNickname())) {
    params.push_back(user->getNickname());
    params.push_back(req.parameter().getParameters().at(0));
    msg = Response::build(ERR_USERNOTINCHANNEL, params,
                          "You're not on the channel!");
    FD_SET(user->getfd(), &fd_write);
    write_cnt = 1;
    return false;
  }
  return true;
}