#ifndef CHANNELMAP_HPP
#define CHANNELMAP_HPP

#include <map>
#include <vector>

#include "Channel.hpp"

class ChannelMap {
 private:
  std::map<std::string, Channel> channelMap;

 public:
  ChannelMap();
  ~ChannelMap();
  void addChannel(Channel channel);
  void deleteChannel(std::string channelName);
  const Channel &findChannel(std::string channelName);
  size_t getSize() const;
  bool exists(std::string channelName);
  std::vector<Channel> findAllChannels() const;
};

#endif