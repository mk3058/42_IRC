#include "Mode.hpp"

Mode::Mode(Request req, User *user) : ICommand(req, user) {
  param = true;
  this->channelMap = &Server::getInstance().getChannelMap();
  if (!req.parameter().getParameters().size()) param = false;
  else if (channelMap->exists(req.parameter().getParameters()[0].substr(1)))
  {
    this->channel = &(
        channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
    this->permission = channel->getMode();
  }
}

bool Mode::kmode(std::string type) {
  std::string key = req.parameter().getParameters()[2];
  int mode = this->channel->getMode();
  if (type.at(0) == '+') {
    if (!checkParam(3))  // 키 문자열을 입력으로 안줌
      return (false);
    else if (mode & KEY_REQURIE)  // 이미 키가 설정된 경우
      return (false);
    this->channel->setPassword(key);
    mode |= KEY_REQURIE;
    this->channel->setMode(mode);
  } else {
    if (!(mode & KEY_REQURIE))  // 이미 채널모드에 키필요 없음
      return (false);
    mode ^= KEY_REQURIE;           // 10 비트 빼기
    this->channel->setMode(mode);  // 바뀐 모드로 셋
    this->channel->setPassword("");
  }
  return (true);
}

bool Mode::imode(std::string type) {
  int mode = this->channel->getMode();
  if (type.at(0) == '+') {
    if (mode & INVITE_ONLY)  // 이미 모드 설정됨
      return (false);
    mode |= INVITE_ONLY;
    this->channel->setMode(mode);
  } else {
    if (!(mode & INVITE_ONLY))  // 이미 모드 빠져있음
      return (false);
    mode ^= INVITE_ONLY;           // 비트 빼기
    this->channel->setMode(mode);  // 바뀐 모드로 셋
  }
  return (true);
}

static bool is_num(std::string str) {
  for (unsigned long i = 0; i < str.size(); ++i) {
    if (!std::isdigit(str[i])) return (false);
  }
  return (true);
}

bool Mode::lmode(std::string type) {
  if (type.at(0) == '+') {
    if (!checkParam(3))  // 인자 안줌
      return (false);
    std::string cntstr = req.parameter().getParameters()[2];
    if (!is_num(cntstr)) return (false);
    std::stringstream ss;
    long long cnt;
    ss << cntstr;
    ss >> cnt;
    if (this->channel->getUserLimit() ==
        cnt)  // 셋팅된 값이랑 새로 셋하려는 값이랑 같음
      return (false);
    if (cnt <= 0) return (false);
    this->channel->setUserLimit(cnt);
  } else {
    if (this->channel->getUserLimit() == UNLIMITED)
      return (false);
    else
      this->channel->setUserLimit(UNLIMITED);
  }
  return (true);
}

bool Mode::omode(std::string type) {
  std::string msg = "";
  if (!checkParam(3))  // 인자 안줌
    return (false);
  std::string targetName = req.parameter().getParameters()[2];
  if (!this->channel->getUsers().exists(targetName))  // 유저 확인
  {
    msg = Response::error(ERR_USERNOTINCHANNEL, *user, &fd_write,
                          "user not in channel");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  int targetFd = this->channel->getUsers().findUser(targetName).getfd();
  int *targetMode = &(this->channel->getUserPermits()[targetFd]);
  if (type.at(0) == '+') {
    if (*targetMode & USERMODE_SUPER) return (false);
    *targetMode |= USERMODE_SUPER;
  } else {
    if (!(*targetMode & USERMODE_SUPER)) return (false);
    *targetMode ^= USERMODE_SUPER;
  }
  return (true);
}

bool Mode::tmode(std::string type) {
int mode = this->channel->getMode();
  if (type.at(0) == '+') {
    if (mode & TOPIC_ONLY_OPERATOR)  // 이미 모드 설정됨
      return (false);
    mode |= TOPIC_ONLY_OPERATOR;
    this->channel->setMode(mode);
  } else {
    if (!(mode & TOPIC_ONLY_OPERATOR))  // 이미 모드 빠져있음
      return (false);
    mode ^= TOPIC_ONLY_OPERATOR;           // 비트 빼기
    this->channel->setMode(mode);  // 바뀐 모드로 셋
  }
  return (true);
}

bool Mode::checkParam(unsigned long cnt) {
  FD_SET(user->getfd(), &fd_write);
  if (req.parameter().getParameters().size() < cnt) {
    std::string msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write,
                                      "need more params");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  return (true);
}

void Mode::execute() {
  if (!checkPermit()) return;
  if (!checkParam(2)) return;
  std::string type = req.parameter().getParameters()[1];
  int flag;
  if (type.at(1) == 'k') flag = kmode(type);
  if (type.at(1) == 'i') flag = imode(type);
  if (type.at(1) == 'l') flag = lmode(type);
  if (type.at(1) == 'o') flag = omode(type);
  if (type.at(1) == 't') flag = tmode(type);
  // 각 모드에서 에러 인경우
  if (!flag) return;
  std::string msg = Response::build(req.command().getCommand(),
                        req.parameter().getParameters(),
                        "", 
                        user->getNickname() + "!" + user->getUsername() + "@" DEFAULT_PREFIX);
  write_cnt = channel->getUsers().getSize();
  for (int i = 0; i < write_cnt; ++i)
    FD_SET(channel->getUsers().findAllUsers()[i]->getfd(), &fd_write);
  Server::getInstance().bufferMessage(msg, write_cnt, &fd_write);
  ;
}

bool Mode::checkPermit() {
  int fd = user->getfd();
  FD_SET(fd, &fd_write);
  // 채널명이 잘못됨
  if (param == false)
  {
    msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write, "parameter error(mode)");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  if (!channelMap->exists(req.parameter().getParameters()[0].substr(1))) {
    msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write,
                          "wrong channel name");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  // 권한확인이 먼저
  int upermission = channel->getUserPermits()[user->getfd()];
  if (!(upermission & USERMODE_SUPER)) {
    msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write,
                          "you are not operator");
    Server::getInstance().bufferMessage(msg, 1, &fd_write);
    return (false);
  }
  return (true);
}
