#ifndef JOIN_HPP
#define JOIN_HPP

#include "ICommand.hpp"
#include <algorithm>

class Join : ICommand
{
    private:
        std::map channelMap;
        join();
    public:
        Join(Request req, User user, std::map& channelMap; fd_set &fd_write);
        void execute();
};

#endif