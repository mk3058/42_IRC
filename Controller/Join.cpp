#include "Join.hpp"

Join::Join(Request req, User user, std::map& channelMap, fd_set &fd_write)
{
    this->req = req;
    this->user = user;
    this->channelMap = channelMap;
    this->fd_write = fd_write;
    if (!is_inChannelMap(req.parameter.getParameters(), channelMap))
        updateChannelMap(req.parameter.getParameters(), channelMap);
    this->channel = channelMap[c_name];
}

Join::execute()
{
    if (user_check())
        return ;
    channel.joinUser(user);
    std::map<int, std::string>::iterator iter;
    for (iter = channel.users.begin(); iter != channel.users.end(); ++iter)
        FD_SET(iter->first, fd_write);
    //리스폰스 메시지 만들기
}

static bool user_check()
{
    return (channel.users[user.getUserName()] || channel.banned_user[user.getUserName()]);
}

static void updateChannelMap(std::string c_name, std::map& channelMap)
{
    Channel channel(c_name);
    channelMap.insert(c_name, channel);
}

static bool is_inChannelMap(std::string c_name, std::map& channelMap)
{
    return (channelMap.count(c_name));
}