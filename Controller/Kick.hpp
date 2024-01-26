#ifndef Kick_HPP
#define Kick_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"

class Kick : public ICommand
{
    private:
        ChannelMap *channelMap;
        Channel channel;
        bool checkPermit();
    public:
        void execute();
        Kick(Request req, User *user);
};

#endif