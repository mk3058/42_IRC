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
        receiveMessage(i);
      }
      changedFdCount--;
    }
    i++;
  }
}

void Server::receiveMessage(int fd) {
  char buffer[513];  // 임시 버퍼

  int nbytes = recv(fd, buffer, 512, 0);
  if (nbytes <= 0) {  // 오류
    // 클라이언트 연결 종료 처리
    close(fd);
    clientBuffers.erase(fd);
    userMap.deleteUser(fd);
    used_fd[fd] = 0;
    std::cout << "client #" << fd << " gone away" << std::endl;
    return;
  }
  buffer[nbytes] = '\0';  // 버퍼 널문자 처리

  // 메시지 버퍼에 데이터 추가
  clientBuffers[fd].append(buffer, nbytes);

  // 완전한 메시지인지 확인
  size_t pos;
  while ((pos = clientBuffers[fd].find("\r\n")) != std::string::npos) {
    std::string message = clientBuffers[fd].substr(0, pos + 2);
    clientBuffers[fd].erase(0, pos + 2);

    // 완전한 메시지일 경우에만 요청 실행
    try {
      Request request(message);
      Controller Controller(request, &(this->userMap.findUser(fd)));
      Controller.execute();
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
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

std::string Server::getPassword() { return this->password; }

int *Server::getUsedfd() { return this->used_fd; }