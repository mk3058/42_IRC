#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP  

#include <string>
#include <map>
#include <stdexcept>
#include <unordered_map>

class IrcStatusCodeHelper
{
    private:
        static          std::unordered_map<std::string, std::string> statusMap;
        static bool     init;
    public:
        static void initialMap();
        static std::string& getStatusCode(const std::string& statusName);
};

#endif