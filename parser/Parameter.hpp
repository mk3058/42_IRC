#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <exception>
#include <string>
#include <vector>

class Parameter {
 private:
  std::vector<std::string> params;
  std::string trailer;

 public:
  Parameter(const std::string &parameter);
  std::vector<std::string> getParameters() const;
  std::string getTrailer() const;
};

#endif