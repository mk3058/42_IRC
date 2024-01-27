#include "Topic.hpp"

Topic::Topic(Request request, User *user)
    : server(Server::getInstance()),
      serverChannels(server.getChannelMap()),
      serverUsers(server.getUserMap()) {
  this->req = request;
  this->user = user;
  this->write_cnt = 0;
  if (req.parameter().isTrailerExists()) {
    this->permission = USERMODE_TOPIC;
  } else {
    this->permission = DEFAULT;
  }
}

void Topic::execute() {
  if (validate()) {
    if (req.parameter().isTrailerExists()) {
      changeTopic();
    } else {
      showTopic();
    }
  }
  server.Send(msg, write_cnt, &fd_write);
}

void Topic::changeTopic() {
  const Parameter &params = req.parameter();
  Channel &channel = serverChannels.findChannel(params.getParameters().at(0));
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
  Channel &channel = serverChannels.findChannel(params.getParameters().at(0));

  msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(), channel.getTopic());
  write_cnt = 1;
  FD_SET(user->getfd(), &fd_write);
}

bool Topic::checkPermit() {
  std::vector<std::string> params = req.parameter().getParameters();
  Channel &channel = serverChannels.findChannel(params.at(0));

  return (this->permission || channel.getUserPermits().at(user->getfd())) ==
         this->permission;
}

bool Topic::validate() {
  if (!checkPermit()) {
    msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  if (req.parameter().getParameters().size() < 1) {
    msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write);
    write_cnt = 1;
    return false;
  }
  return true;
}