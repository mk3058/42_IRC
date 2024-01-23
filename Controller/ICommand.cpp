#include "ICommand.hpp"

bool ICommand::checkPermit()
{
    if (permission == USERMODE_SUPER)
        return (true);
    return (false);
}

std::string ICommand::getResponse() const
{
    return this->msg;
}

int ICommand::getCount() const
{
    return this->write_cnt;
}