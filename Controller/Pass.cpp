#include "Pass.hpp"

Pass::Pass(Request request, User *user) : ICommand(request, user) {}

void Pass::execute()
{
    Server &server = Server::getInstance();
    if (server.auth(req.parameter().getParameters()[0]) == 0)   // 비밀번호 틀린 경우
    {
        this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        if (server.getcerti()[user->getfd()] == -2)     // 3번 틀린 경우
        {
            close(user->getfd());
            server.getUserMap().deleteUser(user->getfd());
            std::cout << user->getfd() << " is disconnected by attempt three times." << std::endl;
        }
        else if (server.getcerti()[user->getfd()] <= 0)
        {
            server.getcerti()[user->getfd()]--;
            std::vector<std::string> param;
            param[0] = "*";
            this->msg = Response::build("PRIVMSG", param, "plz try valid password");
            send(user->getfd(), msg.c_str(), msg.size(), 0);
            std::cout << user->getfd() << " failed the password " << std::endl;  
        }
    }
    else
    {
        server.getcerti()[user->getfd()] = 1;
        std::vector<std::string> param;
        param[0] = "*";
        this->msg = Response::build("PRIVMSG", param, "plz input nickname");
        send(user->getfd(), msg.c_str(), msg.size(), 0);
    }
}

bool Pass::checkPermit()
{
    return (1);
}

std::string Pass::getResponse() const
{
    return msg;
}

int Pass::getCount() const
{
    return msg.size();
}