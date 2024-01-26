#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <sys/select.h>

#include "Request.hpp"
#include "Server.hpp"

class User;

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
