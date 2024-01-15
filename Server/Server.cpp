#include "Server.hpp"

Server::Server(std::string password, int port) {
  this->password = password;
  this->port = port;

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
Server::Server(const Server &ref) {}

/** 싱글톤 패턴을 유지하기 위해 복사생성자, 대입연산자 삭제 */
Server Server::operator=(const Server &rval) {}

Server::~Server() { delete instance; }