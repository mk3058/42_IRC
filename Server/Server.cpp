#include "Server.hpp"

#include <cstring>

Server::Server(std::string password, int port) {
  this->password = password;
  this->port = port;

  memset(used_fd, 0, sizeof(used_fd));
  this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_HOPOPTS);
  if (socket_fd == -1) throw std::runtime_error("Failed socket create");
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  if (bind(socket_fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    throw std::runtime_error("Failed bind");
  if (listen(socket_fd, LISTEN_QUEUE_SIZE) == -1)
    throw std::runtime_error("Failed listen");
  used_fd[socket_fd] = 1;
}

void Server::connect() {
  int cs;
  struct sockaddr_in csin;
  socklen_t csin_len;

  csin_len = sizeof(csin);
  cs = accept(socket_fd, (struct sockaddr *)&csin, &csin_len);
  if (cs == -1) throw std::runtime_error("Failed accept");
  used_fd[cs] = 1;
  std::cout << "New client " << cs << " from " << inet_ntoa(csin.sin_addr)
            << ":" << ntohs(csin.sin_port) << std::endl;
}

void Server::io_multiplex() {
  int i = 0;
  int changedFdCount = 0;

  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  while (i < MAX_USER) {
    if (used_fd[i]) {
      FD_SET(i, &fd_read);
      // if (strlen(e->fds[i].buf_write) > 0) {
      FD_SET(i, &fd_write);
      changedFdCount = changedFdCount > i ? changedFdCount : i;
    }
    i++;
  }
  int r = select(changedFdCount + 1, &fd_read, &fd_write, 0, 0);
  if (r < 0) {
    throw std::runtime_error("Failed select");
  }
  i = 0;
  while ((i < MAX_USER) && (changedFdCount > 0)) {
    if (FD_ISSET(i, &fd_read)) this->test(i);
    // if (FD_ISSET(i, &fd_write)) e->fds[i].fct_write(e, i);
    if (FD_ISSET(i, &fd_read) /*|| FD_ISSET(i, &e->fd_write)*/)
      changedFdCount--;
    i++;
  }
}

void Server::test(int cs) {
  int r;
  int i;
  char buf[4096];

  if (cs == socket_fd) {
    connect();
    return;
  }

  r = recv(cs, buf, 4096, 0);
  if (r < 0) {
    close(cs);
    used_fd[cs] = 0;
    std::cout << "client #" << cs << " gone away" << std::endl;
  } else {
    i = 0;
    while (i < MAX_USER) {
      /*자기랑 서버빼고 보내기
            if ((e->fds[i].type == FD_CLIENT) &&
                (i != cs))*/
      send(i, buf, r, 0);
      i++;
    }
  }
}

Server *Server::instance = NULL;

/**
 * 최초 인스턴스 생성시에 password, port 파라미터와 함께 사용합니다.
 * 인스턴스가 생성된 후 호출시 파라미터를 무시하고 기존 객체를 반환합니다.
 */
Server &Server::getInstance(std::string password, int port) {
  if (instance == NULL) {
    instance = new Server(password, port);
  }

  return *instance;
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