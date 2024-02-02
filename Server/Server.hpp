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
#include "Controller.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "UserMap.hpp"

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
  std::map<int, std::string> recvBuffers;
  std::map<int, std::string> sendBuffers;
  int used_fd[1000];
  int certi[1000];
  int totalUsers;
  int socket_fd;
  time_t serverStartTime;
  struct sockaddr_in sin;
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
  void Send(int cnt);

 public:
  void connect();
  void io_multiplex();
  bool auth(const std::string &password) const;
  UserMap &getUserMap();
  ChannelMap &getChannelMap();
  int &gettotalUsers();
  int *getcerti();
  int *getUsedfd();
  int getsockfd();
  time_t getTime();
  std::string getPassword();
  void receiveMessage(int fd);
  void quitChUser(int fd);
  void delUser(int fd);
  struct sockaddr_in getstruct();
  void bufferMessage(const std::string ResMsg, int write_cnt, fd_set *fd_write);
  static void initialize(std::string password, int port);
  static Server &getInstance();
  ~Server();
};

#endif