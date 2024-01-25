#include "Join.hpp"

Join::Join(Request req, User *user, fd_set *fd_write) : ICommand(req, user, fd_write)
{
    this->channelMap = &Server::getInstance().getChannelMap();
    if (!channelMap->exists(req.parameter().getParameters()[0]))
        channelMap->addChannel(Channel(req.parameter().getParameters()[0]));
    this->channel = channelMap->findChannel(req.parameter().getParameters()[0]);
}

void Join::execute()
{
    if (!checkPermit(user->getfd()))
        return ;
    channel.addUser(*user);
    msg = "join";
}

bool Join::checkPermit(int fd)
{
    if (channel.getUserLimit() == channel.getUsers().getSize())
    {
        msg = Response::error(StatusCode::getStatusCode("ERR_CHANNELISFULL"), *user, fd_write, "channel is full");
        return (false);
    }
    if (channel.getUsers().exists(fd))
        return (false);
    if (channel.getBannedUsers().exists(fd))
    {
        msg = Response::error(StatusCode::getStatusCode("ERR_BANNEDFROMCHAN"), *user, fd_write, "you are banned");
        return (false);
    }
    if (this->permission | INVITE_ONLY && !channel.getInvitedUsers().exists(user->getNickname()))
        msg = Response::error(StatusCode::getStatusCode("ERR_INVITEONLYCHAN"), *user, fd_write, "invite only");
        return (false);
    if (this->permission | KEY_REQURIE)
    {
        if (req.parameter().getParameters().size() < 2)
            msg = Response::error(StatusCode::getStatusCode("ERR_BADCHANNELKEY"), *user, fd_write, "you need key");
        if (channel.getPassword() != req.parameter().getParameters()[1])
            msg = Response::error(StatusCode::getStatusCode("ERR_BADCHANNELKEY"), *user, fd_write, "wrong key");
        return (false);
    }
    return (true);
}
