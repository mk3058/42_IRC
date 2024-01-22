#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <stdexcept>
#include <string>

class Command {
 private:
  std::string command;

 public:
  Command();
  void set(std::string command);
  std::string getCommand() const;
};

#endif