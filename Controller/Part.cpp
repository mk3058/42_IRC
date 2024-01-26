#include "Part.hpp"

Part::Part(Request req, User *user) : ICommand(req, user)
{
    this->channelMap = &Server::getInstance().getChannelMap();
    if (channelMap->exists(req.parameter().getParameters()[0]))
        this->channel = channelMap->findChannel(req.parameter().getParameters()[0]);
    else
        this->channel = Channel();
}

void Part::execute()
{
    if (!checkPermit())
        return ;
    //메시지 모든 채널에 속한 유저에게 전송 자신에게도 전송해야해서 메시지부터 보냄
    msg = Response::build(req.command().getCommand(), req.parameter().getParameters(), "user " + user->getNickname() + "part this channel");
    for(int i = 0; i < channel.getUsers().getSize(); ++i)
        FD_SET(channel.getUsers().findAllUsers()[i]->getfd(), &fd_write);
    Server::getInstance().Send(msg, channel.getUsers().getSize(), &fd_write);
    //유저 지우기
    channel.deleteUser(*user);
    //마지막 유저였으면 채널도 삭제
    if (!channel.getUsers().getSize())
        channelMap->deleteChannel(channel.getName());
}

bool Part::checkPermit()
{
    int fd = user->getfd();
    FD_SET(fd, &fd_write);
    if (channel.getName() == "Not initialized")
    {
        msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write, "wrong channel name");
        Server::getInstance().Send(msg, 1, &fd_write);
        return (false);
    }
    return (true);
}