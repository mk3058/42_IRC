#include "Parameter.hpp"

static const std::string parameterDelimeter = " ";
static const std::string trailerDelimeter = ":";

static void parseParameter(std::string parameter,
                           std::vector<std::string> &token, std::string &tr,
                           bool &TrailerExists) {
  tr = "";
  token.clear();

  // trailer가 존재할 경우 해당 값을 별도로 저장하고 parameter 문자열에서 삭제
  size_t pos;
  if ((pos = parameter.find(trailerDelimeter)) != std::string::npos &&
      !parameter.substr(pos).empty()) {
    tr = parameter.substr(pos + trailerDelimeter.length());
    TrailerExists = !tr.empty();
    parameter.erase(pos);
  }

  // 구분자(" ")를 기준으로 파라미터를 tokenize 하여 주어진 벡터에 저장
  while (true) {
    pos = parameter.find(parameterDelimeter);
    if (!parameter.substr(0, pos).empty()) {  // 빈 문자열이 아닐때만 추가
      token.push_back(parameter.substr(0, pos));
    }
    if (pos == std::string::npos) {
      break;
    }
    parameter.erase(0, pos + parameterDelimeter.length());
  }

  // trailing을 제외한 파라미터의 개수가 14개를 초과할때 예외처리
  if (token.size() > 14) {
    throw std::invalid_argument("파라미터의 개수는 14개 이하여야 합니다.");
  }
}

Parameter::Parameter() : TrailerExists(false) {}

/**
 * Parameter Class
 * Parameter : 파라미터 문자열. 파라미터가 존재하지 않을 경우 빈 문자열
 */
void Parameter::set(const std::string &parameter) {
  if (parameter.empty()) {
    return;
  }
  parseParameter(parameter, params, trailer, TrailerExists);
}

std::vector<std::string> Parameter::getParameters() const { return params; }

std::string Parameter::getTrailer() const { return trailer; }

bool Parameter::isTrailerExists() { return TrailerExists; }