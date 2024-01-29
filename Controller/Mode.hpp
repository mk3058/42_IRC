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
        bool checkParam(int cnt);
        std::string kmode(std::string type);
        std::string imode(std::string type);
    public:
        void execute();
        Mode(Request req, User *user);
};

#endif