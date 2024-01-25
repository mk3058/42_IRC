#ifndef USERCMD_HPP
#define USERCMD_HPP

#include "ICommand.hpp"

class UserCmd : public ICommand
{
    private:

    public:
    UserCmd(Request request, User *user);

    void execute();
    bool checkPermit() { return 1; };
    int checkUser();
    int getCount() const;
    std::string getResponse() const;
    bool checkname(std::string name);
};

#endif