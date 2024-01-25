#ifndef NICK_HPP
#define NICK_HPP

#include "ICommand.hpp"

class Nick : public ICommand
{
    private:

    public:
        Nick(Request request, User *user);

        void execute();
        bool checkpermit() {};
        int checknick();
        std::string getResponse() const;
        int getCount() const;
        bool checkname(std::string name);
};


#endif