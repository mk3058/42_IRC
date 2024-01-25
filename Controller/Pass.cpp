#include "Pass.hpp"

Pass::Pass(Request request, User *user) : ICommand(request, user) {}

void Pass::execute()
{
    Server &server = Server::getInstance();
    if (server.auth(req.parameter().getParameters()[0]) == 0)
    {
        this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        if (server.getcerti()[user->getfd()] == -2)
        {
            close(user->getfd());
            server.getUserMap().deleteUser(user->getfd());
            std::cout << user->getfd() << " is disconnected by attempt three times." << std::endl;
        }
        else if (server.getcerti()[user->getfd()] <= 0)
        {
            std::vector<std::string> param;
            param[0] = "*";
            this->msg = Response::build("PRIVMSG", param, "plz try valid password");
            send(user->getfd(), msg.c_str(), msg.size(), 0);
            std::cout << user->getfd() << " failed the password " << std::endl;  
        }
    }
}

bool Pass::checkPermit()
{

}

std::string Pass::getResponse() const
{

}

int Pass::getCount() const
{

}