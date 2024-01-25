#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <stdexcept>
#include <string>
#include <vector>

class Parameter {
 private:
  std::vector<std::string> params;
  std::string trailer;
  bool TrailerExists;

 public:
  Parameter();
  void set(const std::string &parameter);
  std::vector<std::string> getParameters() const;
  std::string getTrailer() const;
  bool isTrailerExists();
};

#endif