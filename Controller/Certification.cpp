#include "Certification.hpp"

Certification::Certification(Request request, User *user, fd_set *fd_write)
 : ICommand(request, user, fd_write)
{
}


int Certification::getCount() const
{

}


std::string Certification::getResponse() const
{

}

bool        Certification::heckPermit()
{

}


void        Certification::execute()
{
    if (req.command().getCommand() == "CAP" && req.parameter().getParameters()[0] == "LS" 
        && req.parameter().getParameters()[1] == "302")
    {
        if (Server::getInstance().gettotalUsers() > 997)
        {
            this->msg = Response::error("005", *(this->user), fd_write);
        }
        else
        {

        }
    }
}