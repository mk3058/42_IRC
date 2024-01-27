#include "UserCmd.hpp"

UserCmd::UserCmd(Request request, User *user) : ICommand(request, user)
{
}

void UserCmd::execute()
{
    Server &server = Server::getInstance();
    if (!checkPermit())  // 아직 앞 단계 인증이 안됐을때
    {
        this->msg = Response::error(ERR_NOTREGISTERED, *(this->user), &fd_write);
        send(user->getfd(), msg.c_str(), msg.size(), 0);
        std::vector<std::string> param;
        param.push_back("*");
        msg.clear();
        this->msg = Response::build("PRIVMSG", param, "Not yet resistered");
        send(user->getfd(), msg.c_str(), msg.size(), 0); 
    }
    else // 인증 단계일때
    {
        if (!checkname(req.parameter().getParameters()[0])) // 유저 네임 이상할때
        {
            this->msg = Response::error(ERR_NEEDMOREPARAMS, *(this->user), &fd_write);
            send(user->getfd(), msg.c_str(), msg.size(), 0);
        }
        else // 인증 성공 했을 때
        {
            std::vector<std::string> param;
            param.push_back(user->getNickname());
            msg = Response::build(RPL_WELCOME, param, "welcome");
            send(user->getfd(), msg.c_str(), msg.size(), 0);
            server.getcerti()[user->getfd()] = 3;
            std::cout << user->getfd() << "(Client) is resistered" << std::endl;
        }
    }
}


bool UserCmd::checkPermit()
{
    if (Server::getInstance().getcerti()[user->getfd()] < 2)
        return 0;   //비밀번호 및 닉네임 설정이 안됐을때
    return 1;    // 유저네임 인증해야 할때
}

int UserCmd::getCount() const
{
    return msg.size();
}

std::string UserCmd::getResponse() const
{
    return msg;
}

bool    UserCmd::checkname(std::string name)
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