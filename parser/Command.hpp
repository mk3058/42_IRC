#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <exception>
#include <string>

class Command {
 private:
  std::string command;

 public:
  Command(std::string command);
  std::string getCommand() const;
};

#endif