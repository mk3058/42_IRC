#ifndef MODE_HPP
#define MODE_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"

class Mode : public ICommand
{
    private:
        ChannelMap *channelMap;
        Channel *channel;
        bool checkPermit();
    public:
        void execute();
        Mode(Request req, User *user);
};

#endif