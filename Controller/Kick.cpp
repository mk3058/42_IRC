#include "Kick.hpp"

Kick::Kick(Request req, User *user) : ICommand(req, user)
{
    this->channelMap = &Server::getInstance().getChannelMap();
    if (channelMap->exists(req.parameter().getParameters()[0]))
        this->channel = channelMap->findChannel(req.parameter().getParameters()[0]);
    else
        this->channel = Channel();
}

void Kick::execute()
{
    if (!checkPermit())
        return ;
    User *targetUser = &channel.getUsers().findUser(req.parameter().getParameters()[1]);
    //유저 지우기
    channel.deleteUser(*targetUser);
    //마지막 유저였으면 채널도 삭제
    if (!channel.getUsers().getSize())
        channelMap->deleteChannel(channel.getName());
    // 내보내졌다는 메시지 보내기
    msg = Response::build(req.command().getCommand(), req.parameter().getParameters(), "user " + targetUser->getNickname() + "Kick this channel");
    for(int i = 0; i < channel.getUsers().getSize(); ++i)
        FD_SET(channel.getUsers().findAllUsers()[i]->getfd(), &fd_write);
    Server::getInstance().Send(msg, channel.getUsers().getSize(), &fd_write);
}    

bool Kick::checkPermit()
{
    int fd = user->getfd();
    FD_SET(fd, &fd_write);
    // 채널명이 잘못됨 
    if (channel.getName() == "Not initialized")
    {
        msg = Response::error(ERR_NOSUCHCHANNEL, *user, &fd_write, "wrong channel name");
        Server::getInstance().Send(msg, 1, &fd_write);
        return (false);
    }
    //권한확인이 먼저
    int permission = channel.getUserPermits()[user->getfd()];
    if (permission != USERMODE_SUPER)
    {
        msg = Response::error(ERR_CHANOPRIVSNEEDED, *user, &fd_write, "you are not operator");
        Server::getInstance().Send(msg, 1, &fd_write);
        return (false);
    }
    //유저네임이 안들어옴
    if (req.parameter().getParameters().size() < 2)
    {
        msg = Response::error(ERR_NEEDMOREPARAMS, *user, &fd_write, "need user name");
        Server::getInstance().Send(msg, 1, &fd_write);
    }
    //유저네임이 잘못됨
    if (channel.getUsers().exists(req.parameter().getParameters()[2]))
    {
        msg = Response::error(ERR_USERNOTINCHANNEL, *user, &fd_write, "user not in channel");
        Server::getInstance().Send(msg, 1, &fd_write);
    }
    return (true);
}
