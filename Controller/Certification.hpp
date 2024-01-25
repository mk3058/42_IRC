#ifndef CERTIFICATION_HPP
#define CERTIFICATION_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include "Response.hpp"

class Certification : public ICommand
{
    private:

    public:
    Certification(Request request, User *user, fd_set *fd_write);

    int         getCount() const;
    std::string getResponse() const;
    bool        heckPermit();
    void        execute();

};

#endif