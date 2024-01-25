#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <sys/select.h>

#include "Request.hpp"
#include "Server.hpp"
#include "Pass.hpp"
#include "Nick.hpp"
#include "UserCmd.hpp"
#include "Privmsg.hpp"
#include "Join.hpp"
#include "Topic.hpp"

class Controler
{
    private:
        Request     &request;
        User        *user;

    public:
        Controler(Request &req, User *user);
        void        execute();
};

#endif
