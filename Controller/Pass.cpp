#include "Pass.hpp"

Pass::Pass(Request request, User *user) : ICommand(request, user) {}

void Pass::execute()
{

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