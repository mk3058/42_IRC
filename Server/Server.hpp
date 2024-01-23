#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Channel.hpp"
#include "ChannelMap.hpp"
#include "Command.hpp"
#include "UserMap.hpp"
#include "Request.hpp"

#define MAX_USER 1000
#define LISTEN_QUEUE_SIZE 42
#define MAX_PORT 65535
#define MIN_PORT 1
// #define MIN_PORT 49152

#define UNUSED 0
#define CLIENT 1
#define SERVER 2

class Server {
 private:
  int used_fd[1000];
  int socket_fd;
  std::string password;
  int port;
  UserMap userMap;
  ChannelMap channelMap;
  fd_set fd_read;
  fd_set fd_write;
  static Server *instance;
  Server(std::string password, int port);
  Server(const Server &ref);
  Server operator=(const Server &rval);

 public:
  void connect();
  void io_multiplex();
  void test(int cs);
  bool auth(const std::string &password) const;
  UserMap &getUserMap();
  ChannelMap &getChannelMap();
  static Server &getInstance(std::string password, int port);
  static Server &getInstance();
  ~Server();
};

#endif