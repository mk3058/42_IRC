#include "StatusCode.hpp"


std::map<std::string, std::string> IrcStatusCodeHelper::statusMap;

void IrcStatusCodeHelper::InitialMap()
{
    statusMap["RPL_WELCOME"] = "001";
    statusMap["RPL_BOUNCE"] = "005";
    statusMap["RPL_AWAY"] = "301";
    statusMap["RPL_CHANNELMODEIS"] = "324";
    statusMap["RPL_UNIQOPIS"] = "325";
    statusMap["RPL_NOTOPIC"] = "331";
    statusMap["RPL_TOPIC"] = "332";
    statusMap["RPL_INVITING"] = "341";
    statusMap["RPL_INVITELIST"] = "346";
    statusMap["RPL_ENDOFINVITELIST"] = "347";
    // statusMap["RPL_BANLIST"] = "367"; // Banned rule list (MODE)
    // statusMap["RPL_ENDOFBANLIST"] = "368"; // after code 367, to signal completion of the inquiry. (MODE)
    statusMap["ERR_NOSUCHNICK"] = "401";
    // statusMap["ERR_NOSUCHCHANNEL"] = "403"; // no such channel (JOIN, PART, KICK)
    // statusMap["ERR_TOOMANYCHANNELS"] = "405"; // already joined too many channed (JOIN)
    // statusMap["ERR_TOOMANYTARGETS"] = "407"; // too many targets (JOIN)
    // statusMap["ERR_UNAVAILRESOURCE"] = "437"; // nick/channel is temporarily unavailable (JOIN)
    statusMap["ERR_USERNOTINCHANNEL"] = "441"; // target user not in channel (MODE, KICK)
    statusMap["ERR_NOTONCHANNEL"] = "442"; // not on channel (PART, INVITE, KICK)
    statusMap["ERR_USERONCHANNEL"] = "443"; // Already is in channel (invite)
    statusMap["ERR_NEEDMOREPARAMS"] = "461"; // not enough parameters (ALL)
    statusMap["ERR_KEYSET"] = "467"; // channel password is already set (MODE)
    statusMap["ERR_CHANNELISFULL"] = "471"; // (+l) channel already full (JOIN)
    statusMap["ERR_UNKNOWNMODE"] = "472"; // is unknown mode (MODE)
    statusMap["ERR_INVITEONLYCHAN"] = "473"; // (+i) only invited user can join (JOIN)
    statusMap["ERR_BANNEDFROMCHAN"] = "474"; // (+b) banned user cannot join (JOIN)
    statusMap["ERR_BADCHANNELKEY"] = "475"; // (+k) incorrect channel password (JOIN)
    statusMap["ERR_BADCHANMASK"] = "476"; // (channel name incorrect syntax or pattern, length limit) (JOIN, KICK)
    statusMap["ERR_NOCHANMODES"] = "477"; // Channel doesn't support modes (MODE, TOPIC)
    statusMap["ERR_CHANOPRIVSNEEDED"] = "482"; // not channel's operator (MODE, TOPIC, INVITE, KICK)
}


std::string& IrcStatusCodeHelper::GetStatusCode(const std::string& statusName)
{
    const std::map<std::string, std::string>::iterator it = statusMap.find(statusName);
    if (it != statusMap.end()){
        return it->second;
    } else {
        throw std::invalid_argument("Failed found status name");
        }
}