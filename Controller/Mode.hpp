#ifndef MODE_HPP
#define MODE_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"
#include <sstream>

class Mode : public ICommand
{
    private:
        ChannelMap *channelMap;
        Channel *channel;
        bool checkPermit();
        bool checkParam(unsigned long cnt);
        std::string kmode(std::string type);
        std::string imode(std::string type);
        std::string lmode(std::string type);
        std::string otmode(std::string type, int mode);
    public:
        void execute();
        Mode(Request req, User *user);
};

#endif