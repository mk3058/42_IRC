#include "ChannelMap.hpp"

ChannelMap::ChannelMap() {}

ChannelMap::~ChannelMap() {}

void ChannelMap::addChannel(Channel channel) {
  std::pair<std::map<std::string, Channel>::iterator, bool> result =
      channelMap.insert(make_pair(channel.getName(), channel));

  if (!result.second) {
    throw std::invalid_argument("Cannot add Channel!! duplicate key exist");
  }
}

void ChannelMap::deleteChannel(std::string channelName) {
  std::map<std::string, Channel>::iterator result =
      channelMap.find(channelName);
  channelMap.erase(result);
}

Channel &ChannelMap::findChannel(std::string channelName) {
  std::map<std::string, Channel>::iterator result =
      channelMap.find(channelName);

  if (result == channelMap.end()) {
    throw std::invalid_argument("cannot find Channel");
  }
  return result->second;
}

size_t ChannelMap::getSize() const { return this->channelMap.size(); }

bool ChannelMap::exists(std::string channelName) const {
  std::map<std::string, Channel>::const_iterator result =
      channelMap.find(channelName);

  return result != channelMap.end();
}

std::vector<Channel *> ChannelMap::findAllChannels() {
  std::vector<Channel *> channels;

  for (std::map<std::string, Channel>::iterator it = channelMap.begin();
       it != channelMap.end(); ++it) {
    channels.push_back(&(it->second));
  }
  return channels;
}