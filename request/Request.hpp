#ifndef PARSER_HPP
#define PARSER_HPP

#include "Command.hpp"
#include "Parameter.hpp"
#include "Prefix.hpp"

class Request {
 private:
  Prefix pf;
  Command cmd;
  Parameter param;

 public:
  Request();
  Request(const std::string &message);
  Prefix prefix() const;
  Command command() const;
  Parameter parameter() const;
};

#endif