#ifndef PREFIX_HPP
#define PREFIX_HPP

#include <exception>
#include <string>

class Prefix {
 private:
  std::string name;  // 닉네임 또는 서버이름
  std::string username;
  std::string hostname;

 public:
  Prefix(const std::string &prefix);
  std::string getName() const;
  std::string getUsername() const;
  std::string getHostname() const;
};

#endif