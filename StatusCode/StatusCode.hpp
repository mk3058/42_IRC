#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <map>
#include <stdexcept>
#include <string>

#define RPL_WELCOME "001"  // connect is success
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define RPL_ISUPPORT "005"
#define RPL_BOUNCE "010"  // server is already fully
#define RPL_AWAY "301"    // When an invited user is temporarily away (INVITE)
#define RPL_CHANNELMODEIS "324"  // if a user joins the channel, informs the
                                 // current channel mode (MODE, JOIN)
#define RPL_UNIQOPIS \
  "325"  // Returned when the user is an operator upon inquiry. (MODE +o)
#define RPL_NOTOPIC "331"  // When a user queries the topic, and the
                           // topic has not been set. (TOPIC)
#define RPL_TOPIC "332"    // if the topic is set and a user joins the channel,
                           // send a topic message (JOIN, TOPIC)
// #define RPL_BANLIST "367"  // Banned rule list (MODE)
// #define RPL_ENDOFBANLIST "368"  // after code 367, to signal
// completion of the inquiry. (MODE)
#define RPL_INVITING \
  "341"  // when the invited user is successfully in the channel. (INVITE)
#define RPL_INVITELIST "346"  // List of users with invite privileges (MODE +i)
#define RPL_ENDOFINVITELIST \
  "347"  // after code 346, to signal completion of the inquiry (MODE +i)
// #define RPL_EXCEPTLIST "348"  // when the exception list is set and a user
// checks the list. (MODE +e) #define RPL_ENDOFEXCEPTLIST "349"  //
// after code 348, to signal completion of the inquiry (MODE +e)
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"
#define ERR_NOSUCHNICK \
  "401"  // nickname or channel not found on the server (INVITE)
#define ERR_NOSUCHCHANNEL "403"     // no such channel (JOIN, PART, KICK)
#define ERR_TOOMANYCHANNELS "405"   // already joined too many channels (JOIN)
#define ERR_TOOMANYTARGETS "407"    // too many targets (JOIN)
#define ERR_UNKNOWNCOMMAND "421"    // Unknown Command
#define ERR_ERRONEUSNICKNAME "432"  // nickname error
#define ERR_NICKNAMEINUSE "433"     // already exist niname
#define ERR_UNAVAILRESOURCE \
  "437"  // nick/channel is temporarily unavailable (JOIN)
#define ERR_USERNOTINCHANNEL "441"  // target user not in channel (MODE, KICK)
#define ERR_NOTONCHANNEL "442"      // not on channel (PART, INVITE, KICK)
#define ERR_USERONCHANNEL "443"     // Already is in the channel (invite)
#define ERR_NOTREGISTERED "451"     // not resistered yet (NICK, USER)
#define ERR_NEEDMOREPARAMS "461"    // not enough parameters (ALL)
#define ERR_ALREADYREGISTRED "462"
#define ERR_PASSWDMISMATCH "464"  // password fail in connect (PASS)
#define ERR_KEYSET "467"          // channel password is already set (MODE)
#define ERR_CHANNELISFULL "471"   // (+l) channel already full (JOIN)
#define ERR_UNKNOWNMODE "472"     // is an unknown mode (MODE)
#define ERR_INVITEONLYCHAN "473"  // (+i) only invited users can join (JOIN)
#define ERR_BANNEDFROMCHAN "474"  // (+b) banned users cannot join (JOIN)
#define ERR_BADCHANNELKEY "475"   // (+k) incorrect channel password (JOIN)
#define ERR_BADCHANMASK "476"     // (channel name incorrect syntax or
                                  // pattern, length limit) (JOIN, KICK)
#define ERR_NOCHANMODES "477"     // Channel doesn't support modes (MODE, TOPIC)
#define ERR_CHANOPRIVSNEEDED \
  "482"  // not the channel's operator (MODE, TOPIC, INVITE, KICK)
#define ERR_NORECIPIENT "411"  // no recipient (PRIVMSG, NOTICE)
#define ERR_NOTEXTTOSEND "412"  // no text to send (PRIVMSG, NOTICE)
#define ERR_CANNOTSENDTOCHAN "404"  // cannot send to channel (PRIVMSG, NOTICE)
#define PRL_WHOREPLY "354"           // WHO command

#endif
