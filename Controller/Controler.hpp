#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <sys/select.h>

#include "Request.hpp"
#include "Server.hpp"

class Controler
{
    private:
        Request     &request;
        User        user;

    public:
        Controler(Request &request, User *user);
        void        execute();
        
        int         getWriteCnt();
        int         getlengthRs();
        std::string getRespons();


};

#endif
