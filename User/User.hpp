#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
    private:
        int         fd;
        std::string username;
        std::string nickname;

    public:
        User(std::string username, std::string nickname, int fd);
        void setNickname(std::string nickname);
        std::string getUsername() const;
        std::string getNickname() const;
        int getfd() const;
};

#endif