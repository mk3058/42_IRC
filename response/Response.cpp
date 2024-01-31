#include "Response.hpp"

#include <exception>

static const std::string partDelimiter = ":";
static const std::string messageEndingDelimeter = "\r\n";
static const std::string space = " ";

std::string Response::build(std::string responseCode,
                            std::vector<std::string> params,
                            std::string trailer, std::string prefix) {
  if (params.size() > 14) {
    throw std::invalid_argument("파라미터의 개수는 14개 이하여야 합니다.");
  }

  std::string msg = "";

  if (prefix.compare(DEFAULT_PREFIX) && prefix.find("@") == std::string::npos) {
    prefix += "@";
    prefix += DEFAULT_PREFIX;
  }
  // prefix 추가
  msg.append(partDelimiter + prefix + space);
  // 응답 코드 추가
  msg.append(responseCode);
  // 파라미터가 존재할 경우추가
  for (size_t i = 0; i < params.size(); i++) {
    msg.append(space + params.at(i));
  }
  // 트레일러가 존재할 경우 추가
  if (!trailer.empty()) {
    msg.append(space + partDelimiter + trailer);
  }
  // CR-LF 추가
  msg.append(messageEndingDelimeter);

  if (msg.size() > 512) {
    throw std::invalid_argument("메시지의 길이는 512byte 이하여야 합니다.");
  }

  return msg;
}
std::string Response::build(std::string responseCode,
                            std::vector<std::string> params,
                            std::string trailer) {
  return build(responseCode, params, trailer, DEFAULT_PREFIX);
}

std::string Response::build(std::string prefix, std::string responseCode,
                            std::string trailer) {
  std::vector<std::string> para;
  return build(responseCode, para, trailer, prefix);
}

std::string Response::build(std::string responseCode,
                            std::vector<std::string> params) {
  return build(responseCode, params, "");
}

std::string Response::build(std::string responseCode) {
  std::vector<std::string> emptyVec;
  return build(responseCode, emptyVec);
}

std::string Response::error(std::string responseCode, User user,
                            fd_set *fd_write, std::string trailer) {
  std::vector<std::string> nickname;

  FD_ZERO(fd_write);
  FD_SET(user.getfd(), fd_write);
  nickname.push_back(user.getNickname());

  return build(responseCode, nickname, trailer);
}

std::string Response::error(std::string responseCode, User user,
                            fd_set *fd_write) {
  std::vector<std::string> nickname;

  FD_ZERO(fd_write);
  FD_SET(user.getfd(), fd_write);
  nickname.push_back(user.getNickname());

  return build(responseCode, nickname, "");
}