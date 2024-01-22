#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#define DEFAULT_PREFIX "ircserv.com"

#include <string>
#include <vector>

class Response {
 public:
  static std::string build(std::string responseCode, std::string prefix,
                           std::vector<std::string> params,
                           std::string trailer);
  static std::string build(std::string responseCode, std::string prefix,
                           std::vector<std::string> params);
  static std::string build(std::string responseCode, std::string prefix);
  static std::string build(std::string responseCode);
};

#endif