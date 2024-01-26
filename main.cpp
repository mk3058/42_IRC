#include <sstream>

#include "Server.hpp"

int arg_check(int ac, char *port, char *password) {
  if (ac != 3) throw std::invalid_argument("Invalid argument");

  std::stringstream ss(port);
  std::string pass(password);
  int size = pass.size();
  int retPort;
  ss >> retPort;
  if (ss.fail() || !ss.eof()) throw std::runtime_error("Invalid Port");
  if (size < 8 || size > 14) throw std::invalid_argument("Invalid Passwor");
  for (int i = 0; i < size; i++) {
    if (!std::isalnum(pass[i])) throw std::invalid_argument("Invalid Password");
  }
  if (retPort > MAX_PORT || retPort < MIN_PORT)
    throw std::invalid_argument("Invalid Port");
  return (retPort);
}

int main(int ac, char *av[]) {
  (void)ac;
  (void)av;

  try {
    int port = arg_check(ac, av[1], av[2]);
    Server &server = Server::getInstance(av[1], port);

    while (true) {
      server.io_multiplex();
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}