//
// Created by Shihao Jing on 5/31/17.
//

#include "Server.h"

namespace HTTP {
namespace Server {

Server::Server(const std::string &address, const std::string &port, const std::string &doc_root)
    : io_service_(),
      acceptor_(io_service_),
      socket_(io_service_) {

  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  do_accept();
}

void Server::run() {
  io_service_.run();
}

void Server::do_accept() {
  acceptor_.async_accept(socket_,
                         [this](boost::system::error_code ec)
                         {
                           // Check whether the server was stopped by a signal before this
                           // completion handler had a chance to run.
                           if (!acceptor_.is_open())
                           {
                             return;
                           }

                           if (!ec)
                           {
                             // start read from socket
                           }

                           do_accept();
                         });
}


} // namespace Server
} // namespace HTTP