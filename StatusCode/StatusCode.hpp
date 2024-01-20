#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP  

#include <string>
#include <map>
#include <stdexcept>


class IrcStatusCodeHelper
{
    private:
        static std::map<std::string, std::string> statusMap;
    public:
        static void InitialMap();
        static std::string& GetStatusCode(const std::string& statusName);
};

#endif