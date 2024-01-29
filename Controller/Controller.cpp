#include "Controller.hpp"

#include "Join.hpp"
#include "Nick.hpp"
#include "Part.hpp"
#include "Pass.hpp"
#include "Privmsg.hpp"
#include "Topic.hpp"
#include "UserCmd.hpp"

Controller::Controller(Request &req, User *user) : request(req), user(user) {}

void Controller::execute() {
  Server &server = Server::getInstance();
  std::string cmd = request.command().getCommand();
  if (cmd == "PING") {
    send(user->getfd(), "PONG :ircserv.com\r\n",
         sizeof("PONG :ircserv.com\r\n"), 0);
  } else if (cmd == "PONG") {
    std::cout << "Client " << user->getfd() << "PONG" << std::endl;
  } else if (cmd == "QUIT") {
    int i = user->getfd();
    std::cout << "client #" << i << " gone away" << std::endl;
    close(i);
    server.getUserMap().deleteUser(i);
    server.getUsedfd()[i] = 0;
    server.getcerti()[i] = 0;
  } else if (Server::getInstance().getcerti()[user->getfd()] < 3) {
    if (cmd == "PASS") {
      Pass pass(request, this->user);
      pass.execute();
    } else if (cmd == "NICK") {
      Nick nick(request, this->user);
      nick.execute();
    }

    else if (cmd == "USER") {
      UserCmd usercmd(request, this->user);
      usercmd.execute();
    } else if (cmd == "CAP") {
      send(user->getfd(), "CAP * LS :\r\n", sizeof("CAP * LS :\r\n"), 0);
    } else {
      fd_set fd_write;
      FD_ZERO(&fd_write);
      std::string msg = Response::error(ERR_NOTREGISTERED, *(this->user),
                                        &fd_write, "Not yet resistered");
      send(user->getfd(), msg.c_str(), msg.size(), 0);
    }
  } else {
    if (cmd == "JOIN") {
      Join join(request, this->user);
      join.execute();
    } else if (cmd == "PRIVMSG") {
      Privmsg privmsg(request, this->user);
      privmsg.execute();
    } else if (cmd == "PART") {
      Part part(request, this->user);
      part.execute();
    }
    // else if (cmd == "KICK")
    // {
    //     Kick kick(request, this->user);
    //     kick.excute();
    // }
    // else if (cmd == "INVITE")
    // {
    //     Invite invite(request, this->user);
    //     invite.excute();
    // }
    else if (cmd == "TOPIC") {
      Topic topic(request, this->user);
      topic.execute();
    } else if (cmd == "MODE") {
      if (request.parameter().getParameters()[0].find('#') == std::string::npos)
        send(user->getfd(), ":ircserv.com  MODE root :+i\r\n",
             sizeof(":ircserv.com  MODE root :+i\r\n"), 0);
      // Mode mode(request, this->user);
      // mode.excute();
    }
    // else if (cmd == "DCC")
    // {
    //     Dcc dcc(request, this->user);
    //     dcc.excute();
    // }
    // else if (cmd == "BOT")
    // {
    //     Bot bot(request, this->user);
    //     bot.excute();
    // }
    else {
      fd_set fd_write;
      FD_ZERO(&fd_write);
      std::string msg = Response::error(ERR_UNKNOWNCOMMAND, *(this->user),
                                        &fd_write, "Unknown Command");
      send(user->getfd(), msg.c_str(), msg.size(), 0);
    }
  }
}
