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
    protected:
        bool checkPermit();
        void execute();
    public:
        Part(Request req, User *user);
};

#endif