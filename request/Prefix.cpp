#include "Prefix.hpp"

#include <iostream>

static const std::string usernameDelimeter = "!";
static const std::string hostnameDelimeter = "@";
enum { NAME, USERNAME, HOSTNAME };

static bool isValidFormat(std::string prefix) {
  size_t hostnameIdx = prefix.find(hostnameDelimeter);
  size_t usernameIdx = prefix.find(usernameDelimeter);

  // hostname이 존재하지 않고 username만 존재할때에 대한 예외처리
  if (usernameIdx != std::string::npos && hostnameIdx == std::string::npos) {
    return false;
  }
  return true;
}

static void parsePrefix(const std::string &prefix, std::string (&token)[3]) {
  size_t hostnameIdx = prefix.find(hostnameDelimeter);
  size_t usernameIdx = prefix.find(usernameDelimeter);

  if (usernameIdx != std::string::npos) {
    token[NAME] = prefix.substr(0, usernameIdx);
  } else {
    token[NAME] = prefix.substr(0, hostnameIdx);
  }
  if (hostnameIdx != std::string::npos) {
    token[HOSTNAME] = prefix.substr(hostnameIdx + hostnameDelimeter.length());
    if (usernameIdx != std::string::npos) {
      token[USERNAME] =
          prefix.substr(usernameIdx + usernameDelimeter.length(),
                        hostnameIdx - usernameIdx - usernameDelimeter.length());
    }
  }
}

Prefix::Prefix() {}

/** Prefix Class
 * Parameter : prefix 구분자(":")을 제외한 문자열. prefix가 존재하지 않을 경우
 * 빈 문자열 */
void Prefix::set(const std::string &prefix) {
  if (!isValidFormat(prefix)) {
    throw std::invalid_argument("prefix 메시지 형식이 잘못되었습니다.");
  }

  std::string token[3];
  parsePrefix(prefix, token);

  this->name = token[NAME];
  this->username = token[USERNAME];
  this->hostname = token[HOSTNAME];
}

std::string Prefix::getName() const { return name; }

std::string Prefix::getUsername() const { return username; }

std::string Prefix::getHostname() const { return hostname; }
