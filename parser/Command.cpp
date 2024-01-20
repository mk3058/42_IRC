#include "Command.hpp"

#include <algorithm>

bool isValidFormat(const std::string &command) {
  if (command.length() < 1) {
    return false;
  }
  return true;
}

/**
 * command class
 * parameter : 명령어 문자열
 */
Command::Command(std::string command) {
  if (!isValidFormat(command)) {
    throw std::invalid_argument("command 메시지 형식이 잘못되었습니다.");
  }

  std::transform(command.begin(), command.end(), command.begin(), std::toupper);
  this->command = command;
}

std::string Command::getCommand() const { return command; }