#ifndef JOIN_HPP
#define JOIN_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "StatusCode.hpp"

class Join : public ICommand
{
    private:
        ChannelMap *channelMap;
        Channel channel;
    protected:
        bool checkPermit(int fd);
        void execute();
    public:
        Join(Request req, User *user, fd_set *fd_write);

};

#endif
