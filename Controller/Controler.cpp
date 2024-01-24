#include "Controler.hpp"

Controler::Controler(Request &request, fd_set &fd_write)
{
    this->request = request;
    this->fd_write = fd_write;
    msgLength = 0;
    fdCnt = 0;
}

void    Controler::execute()
{
    // if (request.command().getCommand() == "CERTIFICATION")
    // {
    //     Certification certi(request);
    //     responseMessage = certi.getResponse();
    //     fdCnt = certi.getCnt();
    // }
    // else if (request.command().getCommand() == "JOIN")
    // {
    //     Join join(request);
    //     responseMessage = join.getResponse();
    //     fdCnt = join.getCnt();
    // }
    // else if (request.command().getCommand() == "PART")
    // {
    //     Part part(request);
    //     responseMessage = part.getResponse();
    //     fdCnt = part.getCnt();
    // }
    // else if (request.command().getCommand() == "KICK")
    // {
    //     Kick kick(request);
    //     responseMessage = kick.getResponse();
    //     fdCnt = kick.getCnt();
    // }
    // else if (request.command().getCommand() == "INVITE")
    // {
    //     Invite invite(request);
    //     responseMessage = invite.getResponse();
    //     fdCnt = invite.getCnt();
    // }
    // else if (request.command().getCommand() == "TOPIC")
    // {
    //     Topic topic(request);
    //     responseMessage = topic.getResponse();
    //     fdCnt = topic.getCnt();
    // }
    // else if (request.command().getCommand() == "MODE")
    // {
    //     Mode mode(request);
    //     responseMessage = mode.getResponse();
    //     fdCnt = mode.getCnt();
    // }
    // else if (request.command().getCommand() == "DCC")
    // {
    //     Dcc dcc(request);
    //     responseMessage = dcc.getResponse();
    //     fdCnt = dcc.getCnt();
    // }
    // else if (request.command().getCommand() == "BOT")
    // {
    //     Bot bot(request);
    //     responseMessage = bot.getResponse();
    //     fdCnt = bot.getCnt();
    // }
    // else
    // {
    //     responseMessage = "Not found command";
    //     fdCnt = 1;
    // }

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