#include "Request.hpp"

#include <stdexcept>

static const std::string messageDelimeter = " ";
static const std::string messageEndingKeyword = "\r\n";
enum { PREFIX, COMMAND, PARAMETER };

bool isValidFormat(std::string message) {
  // 메시지가 ("\r\n")으로 끝나지 않는 경우에 대한 예외처리
  if (message.find(messageEndingKeyword) !=
      message.length() - messageEndingKeyword.length()) {
    return false;
  }
  return true;
}

void parseRequest(std::string message, std::string (&token)[3]) {
  size_t pos = 0;

  message = message.substr(0, message.find(messageEndingKeyword));
  // prefix 파트가 존재하면 해당 부분 저장 후 삭제
  if (message.at(0) == ':') {
    pos = message.find(messageDelimeter);
    token[PREFIX] = message.substr(1, pos);
    message.erase(0, pos + messageDelimeter.length());
  } else {
    token[PREFIX] = "";
  }

  pos = message.find(messageDelimeter);
  token[COMMAND] = message.substr(0, pos);
  if (pos == std::string::npos) {
    token[PARAMETER] = "";
    return;
  }
  message.erase(0, pos + messageDelimeter.length());

  token[PARAMETER] = message;
}

Request::Request() {}

/** Request class
 * Parameter : 클라이언트로부터 받은 메시지 전문
 */
Request::Request(const std::string &message) {
  if (!isValidFormat(message)) {
    throw std::invalid_argument("요청 메시지 형식이 잘못되었습니다.");
  }

  std::string token[3];

  parseRequest(message, token);
  this->pf.set(token[PREFIX]);
  this->cmd.set(token[COMMAND]);
  this->param.set(token[PARAMETER]);
}

Prefix Request::prefix() const { return pf; }

Command Request::command() const { return cmd; }

Parameter Request::parameter() const { return param; }