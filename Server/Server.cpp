#include "Server.hpp"

#include <fcntl.h>

#include <cstring>

Server::Server(std::string password, int port) {
  int optval = 1;
  this->password = password;
  this->port = port;

  totalUsers = 0;
  memset(certi, 0, sizeof(certi));
  memset(used_fd, 0, sizeof(used_fd));
  this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_HOPOPTS);
  if (socket_fd == -1) {
    throw std::runtime_error("Failed to create socket");
  }
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  fcntl(socket_fd, F_SETFL, O_NONBLOCK);
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  if (bind(socket_fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    throw std::runtime_error("Bind function failed");
  if (listen(socket_fd, LISTEN_QUEUE_SIZE) == -1)
    throw std::runtime_error("Listen function failed");
  used_fd[socket_fd] = 1;
  totalUsers++;
  std::cout << "Server is running on port " << port << std::endl;
}

void Server::connect() {
  int cs;
  struct sockaddr_in csin;
  socklen_t csin_len;

  csin_len = sizeof(csin);
  cs = accept(socket_fd, (struct sockaddr *)&csin, &csin_len);
  fcntl(cs, F_SETFL, O_NONBLOCK);
  if (cs == -1) throw std::runtime_error("Accept function failed");
  if (totalUsers > 997) {
    std::vector<std::string> emptyParam;
    std::string msg = Response::build("NOTICE", emptyParam, "Server is full");
    send(cs, msg.c_str(), msg.size(), 0);
    close(cs);
    std::cout << "New client " << cs << " from " << inet_ntoa(csin.sin_addr)
              << ":" << ntohs(csin.sin_port) << "is refused !" << std::endl;
  } else if (!userMap.exists(cs)) {
    User newUser(cs);
    totalUsers++;
    this->userMap.addUser(cs, newUser);
    used_fd[cs] = 1;
    std::cout << "New client " << cs << " from " << inet_ntoa(csin.sin_addr)
              << ":" << ntohs(csin.sin_port) << std::endl;
  }
}

void Server::io_multiplex() {
  int i = 0;
  int changedFdCount = 0;

  FD_ZERO(&fd_read);
  int n = 0;
  while (i < MAX_USER && n < totalUsers) {
    if (used_fd[i] && fcntl(i, F_GETFL) != -1) {
      FD_SET(i, &fd_read);
      changedFdCount = changedFdCount > i ? changedFdCount : i;
      n++;
    }
    i++;
  }
  int r = select(changedFdCount + 1, &fd_read, &fd_write, 0, 0);
  if (r < 0) {
    throw std::runtime_error("select function failed");
  }
  i = 0;
  while ((i < MAX_USER) && (changedFdCount > 0)) {
    if (FD_ISSET(i, &fd_read)) {
      if (i == socket_fd) {
        this->connect();
      } else {
        char buf[512] = {};
        r = recv(i, buf, 512, 0);
        if (r < 0) {
          std::cout << "client #" << i << " gone away" << std::endl;
          close(i);
          userMap.deleteUser(i);
          used_fd[i] = 0;
          totalUsers--;
        } else {
          std::vector<std::string> packet = requestParse(buf);
          for (size_t k = 0; k < packet.size(); k++) {
            try {
              Request request(packet[k]);
              Controller Controller(request, &(this->userMap.findUser(i)));
              Controller.execute();
            } catch (const std::exception &e) {
              std::cerr << e.what() << '\n';
            }
          }
        }
      }
      changedFdCount--;
    }
    i++;
  }
}

bool Server::auth(const std::string &password) const {
  return !this->password.compare(password);
}

UserMap &Server::getUserMap() { return this->userMap; }

ChannelMap &Server::getChannelMap() { return this->channelMap; }

int *Server::getcerti() { return this->certi; }

int &Server::gettotalUsers() { return this->totalUsers; }

Server *Server::instance = NULL;

/**
 * 최초 인스턴스 생성시에 password, port 파라미터와 함께 사용합니다.
 */
void Server::initialize(std::string password, int port) {
  if (instance == NULL) {
    instance = new Server(password, port);
  }
}

/**
 * 파라미터와 함께 호출해서 인스턴스 초기화 한 후 사용해주세요
 * (instance가 초기화 되지 않은 경우 exception을 throw 합니다)
 */
Server &Server::getInstance() {
  if (instance == NULL) {
    throw std::invalid_argument("instance hasn't been initialized yet!");
  }

  return *instance;
}

/** 싱글톤 패턴을 유지하기 위해 복사생성자, 대입연산자 삭제 */
Server::Server(const Server &ref) { (void)ref; }

/** 싱글톤 패턴을 유지하기 위해 복사생성자, 대입연산자 삭제 */
Server Server::operator=(const Server &rval) {
  (void)rval;
  return *this;
}

Server::~Server() { delete instance; }

void Server::Send(const std::string ResMsg, int write_cnt, fd_set *fd_write) {
  size_t length = ResMsg.size();
  for (int i = 0; i < MAX_USER; i++) {
    if (FD_ISSET(i, fd_write)) {
      send(i, ResMsg.c_str(), length, 0);
      write_cnt--;
    }
    if (!write_cnt) break;
  }
}

std::vector<std::string> Server::requestParse(char *buf) {
  std::vector<std::string> packet;
  std::string temp;
  for (int i = 0; buf[i] != '\0'; i++) {
    temp += buf[i];
    if (temp.size() > 1 && buf[i] == '\n' && buf[i - 1] == '\r') {
      packet.push_back(temp);
      temp.clear();
    }
  }
  if (!temp.empty()) packet.push_back(temp);
  return (packet);
}

std::string Server::getPassword() { return this->password; }

int *Server::getUsedfd() { return this->used_fd; }