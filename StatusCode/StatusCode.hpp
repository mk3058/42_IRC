#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP  

enum class IrcStatusCode
{
    // connect status code

    RPL_WELCOME = 001, //connect is success 
    RPL_BOUNCE = 005, //server is already fully

    RPL_AWAY = 301, // When an invited user is temporarily away (INVITE)
    RPL_CHANNELMODEIS = 324, // if user join channel, informs the current channel mode(MODE, JOIN)
    RPL_UNIQOPIS = 325, // Returned when the user is an operator upon inquiry. (MODE +o)
    RPL_NOTOPIC = 331, // When a user queries the topic, and the topic has not been set. (TOPIC)
    RPL_TOPIC = 332, // if topic is set and user join channel, send a topic massage (JOIN, TOPIC)
    // RPL_BANLIST = 367, // Banned rule list (MODE)
    // RPL_ENDOFBANLIST = 368, // after code 367, to signal completion of the inquiry. (MODE)
    RPL_INVITING = 341, // when invited user is successfully in channel. (INVITE)
    RPL_INVITELIST = 346, // List of users with invite privileges (MODE +i)
    RPL_ENDOFINVITELIST = 347, // after code 346, to signal completion of the inquiry (MODE +i)
    // RPL_EXCEPTLIST = 348, // when exception list is seted and user check the list. (MODE +e)
    // RPL_ENDOFEXCEPTLIST = 349, // after code 348, to signal completion of the inquiry (MODE +e)
    ERR_NOSUCHNICK = 401, // nickname or channel no such in server (INVITE)
    // ERR_NOSUCHCHANNEL = 403, // no such channel (JOIN, PART, KICK)
    // ERR_TOOMANYCHANNELS = 405, //  already joined too many channed (JOIN)
    // ERR_TOOMANYTARGETS = 407, // 같은 이름으로 된 너무 많은 채널에 참여 시도 및 많은 유저에게 메세지 보내기 (JOIN)
    // ERR_UNAVAILRESOURCE = 437, // nick/channel is temporarily unavailable (JOIN)


    ERR_USERNOTINCHANNEL = 441, // // target user not in channel (MODE, KICK)
    ERR_NOTONCHANNEL = 442, // not on channel (PART, INVITE, KICK)
    ERR_USERONCHANNEL = 443, // Already is in channel (invite)
    ERR_NEEDMOREPARAMS = 461, // not enough parameters (ALL)
    ERR_KEYSET = 467, // channel password is already seted. (MODE)
    ERR_CHANNELISFULL = 471, // (+l) channel already fully (JOIN)
    ERR_UNKNOWNMODE = 472, // is unknown mode (MODE)
    ERR_INVITEONLYCHAN = 473, // (+i) only invited user can join (JOIN)
    ERR_BANNEDFROMCHAN = 474, // (+b) banned user can not join (JOIN)
    ERR_BADCHANNELKEY = 475, // (+k) incorrectly channel password (JOIN)
    ERR_BADCHANMASK = 476, // (채널 이름 잘못된 문법이나 패턴, 길이 제한) (JOIN, KICK)
    ERR_NOCHANMODES = 477, // Channel doesn't support modes (MODE, TOPIC)
    ERR_CHANOPRIVSNEEDED = 482, // not channel's operator (MODE, TOPIC, INVITE, KICK)

};

#include <string>
#include <map>
#include <stdexcept>

class IrcStatusCodeHelper {
public:
    static int GetCodeFromString(const std::string& statusName) {
        static const std::map<std::string, int> statusMap = {
            {"RPL_WELCOME", 001},
            {"RPL_BOUNCE", 005},
            {"RPL_AWAY", 301},
            {"RPL_CHANNELMODEIS", 324},
            {"RPL_UNIQOPIS", 325},
            {"RPL_NOTOPIC", 331},
            {"RPL_TOPIC", 332},
            {"RPL_INVITING", 341},
            {"RPL_INVITELIST", 346},
            {"RPL_ENDOFINVITELIST", 347},
            {"ERR_NOSUCHNICK", 401},
            {"ERR_USERNOTINCHANNEL", 441},
            {"ERR_NOTONCHANNEL", 442},
            {"ERR_USERONCHANNEL", 443},
            {"ERR_NEEDMOREPARAMS", 461},
            {"ERR_KEYSET", 467},
            {"ERR_CHANNELISFULL", 471},
            {"ERR_UNKNOWNMODE", 472},
            {"ERR_INVITEONLYCHAN", 473},
            {"ERR_BANNEDFROMCHAN", 474},
            {"ERR_BADCHANNELKEY", 475},
            {"ERR_BADCHANMASK", 476},
            {"ERR_NOCHANMODES", 477},
            {"ERR_CHANOPRIVSNEEDED", 482}
        };

        std::map<std::string, int>::iterator it = statusMap.find(statusName);
        if (it != statusMap.end()) {
            return it->second;
        } else {
            throw std::invalid_argument("Failid found status name");
        }
    }
};

#endif