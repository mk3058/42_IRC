#include "Command.hpp"

#include <algorithm>

bool isValidFormat(const std::string& command) {
  if (command.length() < 1) {
    return false;
  }
  return true;
}

std::string toUpperCase(const std::string& str) {
  std::string upperStr = str;
  for (size_t i = 0; i < upperStr.length(); ++i) {
    upperStr[i] = std::toupper(upperStr[i]);
  }
  return upperStr;
}

Command::Command() {}

/**
 * command class
 * parameter : 명령어 문자열
 */
void Command::set(std::string command) {
  if (!isValidFormat(command)) {
    throw std::invalid_argument("command 메시지 형식이 잘못되었습니다.");
  }

  this->command = toUpperCase(command);
}

std::string Command::getCommand() const { return command; }