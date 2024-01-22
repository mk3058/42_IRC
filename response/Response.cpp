#include "Response.hpp"

#include <exception>

static const std::string partDelimiter = ":";
static const std::string messageEndingDelimeter = "\r\n";
static const std::string space = " ";

std::string Response::build(std::string responseCode, std::string prefix,
                            std::vector<std::string> params,
                            std::string trailer) {
  if (params.size() > 14) {
    throw std::invalid_argument("파라미터의 개수는 14개 이하여야 합니다.");
  }

  std::string msg = "";

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

std::string Response::build(std::string responseCode, std::string prefix,
                            std::vector<std::string> params) {
  return build(responseCode, prefix, params, "");
}

std::string Response::build(std::string responseCode, std::string prefix) {
  std::vector<std::string> emptyVector;
  return build(responseCode, prefix, emptyVector);
}

std::string Response::build(std::string responseCode) {
  return build(responseCode, DEFAULT_PREFIX);
}