#include "Notice.hpp"

void Notice::toChannel(std::string channelName, std::string message) {
  Server &server = server.getInstance();

  if (!server.getChannelMap().exists(channelName)) {
    std::cout << "Notice: channel not exists" << std::endl;
  }
  Channel &channel = server.getChannelMap().findChannel(channelName);
  std::vector<User *> channelUsers = channel.getUsers().findAllUsers();
  int noticeCnt = channelUsers.size();
  fd_set fd_write;

  FD_ZERO(&fd_write);
  for (size_t i = 0; i < channelUsers.size(); i++) {
    int fd = channelUsers.at(i)->getfd();
    FD_SET(fd, &fd_write);
  }
  server.bufferMessage(message, noticeCnt, &fd_write);
}

void Notice::toUser(std::string userName, std::string message) {
  Server &server = server.getInstance();

  if (!server.getUserMap().exists(userName)) {
    std::cout << "Notice: user not exists" << std::endl;
  }
  User &user = server.getUserMap().findUser(userName);
  int noticeCnt = 1;
  fd_set fd_write;

  FD_ZERO(&fd_write);
  FD_SET(user.getfd(), &fd_write);
  server.bufferMessage(message, noticeCnt, &fd_write);
}