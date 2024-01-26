#ifndef PART_HPP
#define PART_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"

class Part : public ICommand
{
    private:
        ChannelMap *channelMap;
        Channel channel;
        bool checkPermit();
    public:
        void execute();
        Part(Request req, User *user);
};

#endif