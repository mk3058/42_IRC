#include "Nick.hpp"

Nick::Nick(Request request, User *user) : ICommand(request, user)
{
}

void Nick::execute()
{
    Server &server = Server::getInstance();
    if (req.parameter().getParameters().size() != 1)
    {
        this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        std::vector<std::string> param;
        param[0] = "*";
        this->msg = Response::build("PRIVMSG", param, "plz input valid parameter");
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        std::cout << user->getfd() << " try invalid parameter " << std::endl;
        return ;
    }
    if (checkPermit() == 1)
    {
        if (server.getUserMap().exists(req.parameter().getParameters()[0])) // 닉네임 이미 있을때
        {
            this->msg = Response::error(ERR_NICKNAMEINUSE, *(this->user), &fd_write);
            send(user->getfd(), msg.c_str(), msg.size(), 0);
            std::vector<std::string> param;
            param[0] = "*";
            this->msg = Response::build("PRIVMSG", param, "already exist nickname");
            send(user->getfd(), msg.c_str(), msg.size(), 0);
            std::cout << user->getfd() << " try an already existing nickname " << std::endl;  
        }
        else
        {
            if (!checkname(req.parameter().getParameters()[0]))
            {
                this->msg = Response::build(user->getNickname(), "NICK", req.command().getCommand());
                send(user->getfd(), msg.c_str(), msg.size(), 0);
                User &u = server.getUserMap().findUser(this->user->getfd());
                u.setNickname(req.parameter().getParameters()[0]);
                server.getcerti()[u.getfd()]++;
            }
        }
    }
    else if (checkPermit() == -1)
    {
        this->msg = Response::error(ERR_PASSWDMISMATCH, *(this->user), &fd_write);
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        server.getcerti()[user->getfd()]--;
        std::vector<std::string> param;
        param[0] = "*";
        this->msg = Response::build("PRIVMSG", param, "plz try valid password");
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        std::cout << user->getfd() << " failed the password " << std::endl;  
    }
    else
    {
        std::vector<std::string> param;
        param[0] = "*";
        this->msg = Response::build("PRIVMSG", param, " is already seted nickname");
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        std::cout << user->getfd() << " is already seted nickname" << std::endl; 
    }
}

int Nick::checknick()
{
    if (Server::getInstance().getcerti()[user->getfd()] < 1)
        return -1;
    else if (Server::getInstance().getcerti()[user->getfd()] > 1)
        return 0;
    return 1;
}

std::string Nick::getResponse() const
{
    return (msg);
}

int Nick::getCount() const
{
    return (msg.size());
}

bool Nick::checkname(std::string name)
{
    int len = name.size();
    if (len <= 1 || len >= 10)
        return false;
    for (int i = 0; i < len; i++)
    {
        if (!std::isalnum(name[i]))
            return false;
    }
    return true;
}