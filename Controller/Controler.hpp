#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include <sys/select.h>

#include "Request.hpp"

class Controler
{
    private:
        Request     &request;
        std::string &responseMessage;
        int         msgLength;
        int         fdCnt;
        fd_set      &fd_write;

    public:
        Controler(Request &request, fd_set &fd_write);
        
        void        execute();
        
        int         getWriteCnt();
        int         getlengthRs();
        std::string getRespons();


};

#endif
