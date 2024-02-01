#include "Controller.hpp"

#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
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
  fd_set fd_write;
  FD_ZERO(&fd_write);

  if (cmd == "CAP" && request.parameter().getParameters()[0] == "LS") {
    FD_SET(user->getfd(), &fd_write);
    Server::getInstance().bufferMessage("CAP * LS :\r\n", 1, &fd_write);
  } else if (cmd == "PING") {
    FD_SET(user->getfd(), &fd_write);
    Server::getInstance().bufferMessage("PONG :ircserv.com\r\n", 1, &fd_write);
  } else if (cmd == "PONG") {
    std::cout << "Client " << user->getfd() << "PONG" << std::endl;
  } else if (cmd == "QUIT") {
    server.quitChUser(user->getfd());
    server.delUser(user->getfd());
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
      FD_SET(user->getfd(), &fd_write);
      Server::getInstance().bufferMessage("CAP * LS :\r\n", 1, &fd_write);
    } else {
      fd_set fd_write;
      FD_ZERO(&fd_write);
      std::string msg = Response::error(ERR_NOTREGISTERED, *(this->user),
                                        &fd_write, "Not yet resistered");
      FD_SET(user->getfd(), &fd_write);
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
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
    } else if (cmd == "KICK") {
      Kick kick(request, this->user);
      kick.execute();
    } else if (cmd == "INVITE") {
      Invite invite(request, this->user);
      invite.execute();
    } else if (cmd == "TOPIC") {
      Topic topic(request, this->user);
      topic.execute();
    } else if (cmd == "MODE") {
      Mode mode(request, this->user);
      mode.execute();
    } else {
      std::string msg = Response::error(ERR_UNKNOWNCOMMAND, *(this->user),
                                        &fd_write, "Unknown Command");
      FD_SET(user->getfd(), &fd_write);
      Server::getInstance().bufferMessage(msg, 1, &fd_write);
    }
  }
}

