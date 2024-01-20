#include "Request.hpp"

static const std::string messageDelimeter = " ";
enum { PREFIX, COMMAND, PARAMETER };

void parseRequest(std::string message, std::string (&token)[3]) {
  size_t pos = 0;

  //prefix 파트가 존재하면 해당 부분 저장 후 삭제
  if (message.at(0) == ':') {
    pos = message.find(messageDelimeter);
    token[PREFIX] = message.substr(1, pos);
    message.erase(0, pos + messageDelimeter.length());
  } else {
    token[PREFIX] = "";
  }
  
  pos = message.find(messageDelimeter);
  token[COMMAND] = message.substr(0, pos);
  message.erase(0, pos + messageDelimeter.length());

  token[PARAMETER] = message;
}

/** Request class
 * Parameter : 클라이언트로부터 받은 메시지 전문
 */
Request::Request(const std::string &message) {
  std::string token[3];

  parseRequest(message, token);
  this->pf.set(token[PREFIX]);
  this->cmd.set(token[COMMAND]);
  this->param.set(token[PARAMETER]);
}

Prefix Request::prefix() const { return pf; }

Command Request::command() const { return cmd; }

Parameter Request::parameter() const { return param; }