#include "Controler.hpp"

Controler::Controler(Request &requeste, User *user)
{
    this->request = request;
    msgLength = 0;
    fdCnt = 0;
}

void    Controler::execute()
{
    std::string cmd = request.command().getCommand();
    if (Server::getInstance().)
    {
    if (cmd == "PASS")
    {
        Certification certi(request);
        responseMessage = certi.getResponse();
        fdCnt = certi.getCnt();
    }
    else if (cmd == "NICK")
    {
        
        Certification certi(request);
        responseMessage = certi.getResponse();
        fdCnt = certi.getCnt();
    }
    
    else if (cmd == "USER")
    {
        Certification certi(request);
        responseMessage = certi.getResponse();
        fdCnt = certi.getCnt();
    }
    }
    else if (cmd == "JOIN")
    {
        Join join(request);
        responseMessage = join.getResponse();
        fdCnt = join.getCnt();
    }
    else if (cmd == "PART")
    {
        Part part(request);
        responseMessage = part.getResponse();
        fdCnt = part.getCnt();
    }
    else if (cmd == "KICK")
    {
        Kick kick(request);
        responseMessage = kick.getResponse();
        fdCnt = kick.getCnt();
    }
    else if (cmd == "INVITE")
    {
        Invite invite(request);
        responseMessage = invite.getResponse();
        fdCnt = invite.getCnt();
    }
    else if (cmd == "TOPIC")
    {
        Topic topic(request);
        responseMessage = topic.getResponse();
        fdCnt = topic.getCnt();
    }
    else if (cmd == "MODE")
    {
        Mode mode(request);
        responseMessage = mode.getResponse();
        fdCnt = mode.getCnt();
    }
    else if (cmd == "DCC")
    {
        Dcc dcc(request);
        responseMessage = dcc.getResponse();
        fdCnt = dcc.getCnt();
    }
    else if (cmd == "BOT")
    {
        Bot bot(request);
        responseMessage = bot.getResponse();
        fdCnt = bot.getCnt();
    }
    else
    {
        responseMessage = "Not found command";
        fdCnt = 1;
    }
    }

    msgLength = responseMessage.size();
}


int Controler::getlengthRs()
{
    return (msgLength);
}

int Controler::getWriteCnt()
{
    return (fdCnt);
}

std::string Controler::getRespons()
{
    return (responseMessage);
}