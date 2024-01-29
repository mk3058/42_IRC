#include "Mode.hpp"

Mode::Mode(Request req, User *user) : ICommand(req, user)
{
    this->channelMap = &Server::getInstance().getChannelMap();
    if (channelMap->exists(req.parameter().getParameters()[0].substr(1)))
        this->channel = &(channelMap->findChannel(req.parameter().getParameters()[0].substr(1)));
    else
    {   
        Channel temp = Channel();
        this->channel = &temp;
    }
    this->permission = channel->getMode();
}

void Mode::execute()
{
    if (!checkPermit())
        return ;
    std::string type = req.parameter().getParameters()[1];
    
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
