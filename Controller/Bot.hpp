#ifndef BOT_HPP
#define BOT_HPP

#include "Request.hpp"

class Bot
{
    private:
    std::vector<std::string> menu;
    int                      user_fd;
    int                      unbalancedCnt;
    Request                  req;
    std::string              nickname;

    public:
    Bot(Request req, int fd, std::string nickname);
    void execute();
    void menuInit();
    std::string findmenu();
};

#endif