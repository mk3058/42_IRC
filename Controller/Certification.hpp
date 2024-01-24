#ifndef CERTIFICATION_HPP
#define CERTIFICATION_HPP

#include "ICommand.hpp"
#include "Server.hpp"


class Certification : public ICommand
{
    private:

    public:
    Certification(Request requst, User *user, fd_set *fd_write);

    void            excute();
    bool            checkPermit();
    std::string     getResponse() const;
    int             getCount() const;
};

#endif

