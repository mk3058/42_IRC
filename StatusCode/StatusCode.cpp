#include "StatusCode.hpp"

std::map<std::string, std::string> StatusCode::statusMap;
bool StatusCode::init = false;

void StatusCode::initialMap() {
  statusMap["RPL_WELCOME"] = "001";  // connect is success
  statusMap["RPL_BOUNCE"] = "005";   // server is already fully
  statusMap["RPL_AWAY"] =
      "301";  // When an invited user is temporarily away (INVITE)
  statusMap["RPL_CHANNELMODEIS"] = "324";  // if user join channel, informs the
                                           // current channel mode (MODE, JOIN)
  statusMap["RPL_UNIQOPIS"] =
      "325";  // Returned when the user is an operator upon inquiry. (MODE +o)
  statusMap["RPL_NOTOPIC"] = "331";  // When a user queries the topic, and the
                                     // topic has not been set. (TOPIC)
  statusMap["RPL_TOPIC"] = "332";    // if topic is set and user join channel,
                                     // send a topic massage (JOIN, TOPIC)
  // statusMap["RPL_BANLIST"] = "367"; // Banned rule list (MODE)
  // statusMap["RPL_ENDOFBANLIST"] = "368"; // after code 367, to signal
  // completion of the inquiry. (MODE)
  statusMap["RPL_INVITING"] =
      "341";  // when invited user is successfully in channel. (INVITE)
  statusMap["RPL_INVITELIST"] =
      "346";  // List of users with invite privileges (MODE +i)
  statusMap["RPL_ENDOFINVITELIST"] =
      "347";  // after code 346, to signal completion of the inquiry (MODE +i)
  // statusMap["RPL_EXCEPTLIST"] = "348"; // when exception list is set and user
  // check the list. (MODE +e) statusMap["RPL_ENDOFEXCEPTLIST"] = "349"; //
  // after code 348, to signal completion of the inquiry (MODE +e)
  statusMap["ERR_NOSUCHNICK"] =
      "401";  // nickname or channel no such in server (INVITE)
statusMap["ERR_NOSUCHCHANNEL"] = "403"; // no such channel (JOIN, PART,
  // KICK) statusMap["ERR_TOOMANYCHANNELS"] = "405"; // already joined too many
  // channed (JOIN) statusMap["ERR_TOOMANYTARGETS"] = "407"; // too many targets
  // (JOIN) statusMap["ERR_UNAVAILRESOURCE"] = "437"; // nick/channel is
  // temporarily unavailable (JOIN)
  statusMap["ERR_USERNOTINCHANNEL"] =
      "441";  // target user not in channel (MODE, KICK)
  statusMap["ERR_NOTONCHANNEL"] = "442";  // not on channel (PART, INVITE, KICK)
  statusMap["ERR_USERONCHANNEL"] = "443";   // Already is in channel (invite)
  statusMap["ERR_NEEDMOREPARAMS"] = "461";  // not enough parameters (ALL)
  statusMap["ERR_KEYSET"] = "467";  // channel password is already set (MODE)
  statusMap["ERR_CHANNELISFULL"] = "471";  // (+l) channel already full (JOIN)
  statusMap["ERR_UNKNOWNMODE"] = "472";    // is unknown mode (MODE)
  statusMap["ERR_INVITEONLYCHAN"] =
      "473";  // (+i) only invited user can join (JOIN)
  statusMap["ERR_BANNEDFROMCHAN"] =
      "474";  // (+b) banned user cannot join (JOIN)
  statusMap["ERR_BADCHANNELKEY"] =
      "475";  // (+k) incorrect channel password (JOIN)
  statusMap["ERR_BADCHANMASK"] = "476";  // (channel name incorrect syntax or
                                         // pattern, length limit) (JOIN, KICK)
  statusMap["ERR_NOCHANMODES"] =
      "477";  // Channel doesn't support modes (MODE, TOPIC)
  statusMap["ERR_CHANOPRIVSNEEDED"] =
      "482";  // not channel's operator (MODE, TOPIC, INVITE, KICK)
  init = true;
}

std::string& StatusCode::getStatusCode(const std::string& statusName) {
  if (init == false)
    throw std::invalid_argument("statusMap is not initializied");
  const std::map<std::string, std::string>::iterator it =
      statusMap.find(statusName);
  if (it != statusMap.end()) {
    return it->second;
  } else {
    throw std::invalid_argument("Failed found status name");
  }
}