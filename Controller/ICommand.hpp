#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <iostream>
#include "Channel.hpp"
#include "User.hpp"
#include "Request.hpp"

class ICommand
{
    protected:
        Request req;
        User    user;
        Channel channel;
        int permission;
        int write_cnt;
        fd_set &fd_write
        std::string msg;
    public:
        virtual void checkPermit();
        virtual std::string getResponse() const;
        virtual int getCount() const;
        virtual void execute() = 0;
};

#endif