#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <iostream>
#include <stdexcept>
#include <string>

#include "Channel.hpp"
#include "UserMap.hpp"

#define MAX_USER 1000
#define LISTEN_QUEUE_SIZE 42
#define MAX_PORT 65535
#define MIN_PORT 49152

class Server {
 private:
  int socket_fd;
  std::string password;
  int port;
  UserMap userMap;
  std::vector<Channel> channels;
  fd_set fd_read;
  fd_set fd_write;
  int changedFdCount;
  static Server *instance;
  Server(std::string password, int port);
  Server(const Server &ref);
  Server operator=(const Server &rval);

 public:
  static Server &getInstance(std::string password, int port);
  static Server &getInstance();
  ~Server();
};

#endif