#include "Mode.hpp"

Mode::Mode(Request req, User *user) : ICommand(req, user)
{
    this->channelMap = &Server::getInstance().getChannelMap();
    if (req.parameter().getParameters().size() < 1)
    {
        Channel temp = Channel();
        this->channel = &temp;
    }
    else if (channelMap->exists(req.parameter().getParameters()[0].substr(1)))
        this->channel = &(channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
    else
    {   
        Channel temp = Channel();
        this->channel = &temp;
    }
    this->permission = channel->getMode();
}

std::string Mode::kmode(std::string type)
{
    std::string msg = "";
    std::string key = req.parameter().getParameters()[2];
    int mode = this->channel->getMode();
    if (type.at(0) == '+')
    {
        if (!checkParam(3)) //키 문자열을 입력으로 안줌
            return msg;
        else if (mode & KEY_REQURIE) //이미 키가 설정된 경우
            return msg;
        this->channel->setPassword(key);
        mode |= KEY_REQURIE;
        this->channel->setMode(mode);
    }
    else
    {
        if (!(mode & KEY_REQURIE)) //이미 채널모드에 키필요 없음
            return msg;
        mode ^= KEY_REQURIE; // 10 비트 빼기
        this->channel->setMode(mode); //바뀐 모드로 셋
        this->channel->setPassword("");
    }
    msg = Response::build(req.command().getCommand()
                        , req.parameter().getParameters()
                        , "user " + user->getNickname() + type);
    return (msg);
}

std::string Mode::imode(std::string type)
{
    std::string msg = "";
    int mode = this->channel->getMode();
    if (type.at(0) == '+')
    {
        if (mode & INVITE_ONLY) //이미 모드 설정됨
            return msg;
        mode |= INVITE_ONLY;
        this->channel->setMode(mode);
    }
    else
    {
        if (!(mode & INVITE_ONLY)) //이미 모드 빠져있음
            return msg;
        mode ^= INVITE_ONLY; // 비트 빼기
        this->channel->setMode(mode); //바뀐 모드로 셋
    }
    msg = Response::build(req.command().getCommand()
                        , req.parameter().getParameters()
                        , "user " + user->getNickname() + type);
    return (msg);
}

static bool is_num(std::string str)
{
    for(unsigned long i = 0; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

std::string Mode::lmode(std::string type)
{
    std::string msg = "";
    if (type.at(0) == '+')
    {
        if (!checkParam(3)) //인자 안줌
            return msg;
        std::string cntstr = req.parameter().getParameters()[2];
        if (!is_num(cntstr))
            return msg;
        std::stringstream ss;
        long long cnt;
        ss << cntstr;
        ss >> cnt;
        if (this->channel->getUserLimit() == cnt) // 셋팅된 값이랑 새로 셋하려는 값이랑 같음
            return msg;
        if (cnt <= 0)
            return msg;
        this->channel->setUserLimit(cnt);
    }
    else
    {
        if (this->channel->getUserLimit() == UNLIMITED)
            return msg;
        else
            this->channel->setUserLimit(UNLIMITED);
    }
    msg = Response::build(req.command().getCommand()
                        , req.parameter().getParameters()
                        , "user " + user->getNickname() + type);
    return (msg);
}

std::string Mode::otmode(std::string type, int mode)
{
    std::string msg = "";
    if (!checkParam(3)) //인자 안줌
        return msg;
    std::string targetName = req.parameter().getParameters()[2];
    if (!this->channel->getUsers().exists(targetName)) // 유저 확인
    {
        msg = Response::error(ERR_USERNOTINCHANNEL, *user, &fd_write, "user not in channel");
        Server::getInstance().Send(msg, 1, &fd_write);
        return "";
    }
    int         targetFd = this->channel->getUsers().findUser(targetName).getfd();
    int         *targetMode = &(this->channel->getUserPermits()[targetFd]);
    if (type.at(0) == '+')
    {
        if (*targetMode & mode)
            return msg;
        *targetMode |= mode;
    }
    else
    {
        if (!(*targetMode & mode))
            return msg;
        *targetMode ^= mode;
    }
    msg = Response::build(req.command().getCommand()
                        , req.parameter().getParameters()
                        , "user " + user->getNickname() + type);
    return (msg);
}

bool Mode::checkParam(unsigned long cnt)
{
    FD_SET(user->getfd(), &fd_write);
    if (req.parameter().getParameters().size() < cnt)
    {
        std::string msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write, "need more params");
        Server::getInstance().Send(msg, 1, &fd_write);
        return (false);
    }
    return (true);
}

void Mode::execute()
{
    if (!checkPermit())
        return ;
    if (!checkParam(2))
        return ;
    std::string type = req.parameter().getParameters()[1];
    std::string msg;
    if (type.find('k'))
        msg = kmode(type);
    if (type.find('i'))
        msg = imode(type);
    if (type.find('l'))
        msg = lmode(type);
    if (type.find('o'))
        msg = otmode(type, USERMODE_SUPER);
    if (type.find('t'))
        msg = otmode(type, USERMODE_TOPIC);
    //각 모드에서 에러 인경우
    if (msg == "")
        return ;
    for (int i = 0; i < channel->getUsers().getSize(); ++i)
        FD_SET(channel->getUsers().findAllUsers()[i]->getfd(), &fd_write);
    Server::getInstance().Send(msg, channel->getUsers().getSize(), &fd_write);
}

bool Mode::checkPermit()
{
    int fd = user->getfd();
    FD_SET(fd, &fd_write);
    // 채널명이 잘못됨 
    if (channel->getName() == "Not initialized")
    {
        msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write, "wrong channel name");
        Server::getInstance().Send(msg, 1, &fd_write);
        return (false);
    }
    //권한확인이 먼저
    int upermission = channel->getUserPermits()[user->getfd()];
    if (upermission != USERMODE_SUPER)
    {
        msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write, "you are not operator");
        Server::getInstance().Send(msg, 1, &fd_write);
        return (false);
    }
    return (true);
}
