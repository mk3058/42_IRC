#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#define DEFAULT_PREFIX "ircserv.com"

#include <sys/select.h>

#include <string>
#include <vector>

#include "User.hpp"

class Response {
 public:
  static std::string build(std::string responseCode,
                           std::vector<std::string> params, std::string trailer,
                           std::string prefix);
  static std::string build(std::string prefix, std::string responseCode,
                           std::string trailer);
  static std::string build(std::string responseCode,
                           std::vector<std::string> params,
                           std::string trailer);
  static std::string build(std::string responseCode,
                           std::vector<std::string> params);
  static std::string build(std::string responseCode);

  static std::string error(std::string responseCode, User user,
                           fd_set *fd_write, std::string trailer);
  static std::string error(std::string responseCode, User user,
                           fd_set *fd_write);
};

#endif