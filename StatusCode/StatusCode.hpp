#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP  

#include <string>
#include <map>
#include <stdexcept>

class StatusCode
{
    private:
        static          std::map<std::string, std::string> statusMap;
        static bool     init;
    public:
        static void initialMap();
        static std::string& getStatusCode(const std::string& statusName);
};

#endif