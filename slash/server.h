//
// Created by Shihao Jing on 5/31/17.
//

#ifndef ZION_SERVER_H
#define ZION_SERVER_H

#include <boost/asio.hpp>
#include <string>
#include "connection_manager.h"
#include "app.h"

namespace zion {

template <typename Handler>
class Server {
public:
  Server(const std::string &address, const std::string &port, const std::string &doc_root, Handler *app)
      : io_service_(),
        acceptor_(io_service_),
        socket_(io_service_),
        app_(app)
  {

    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    do_accept();
  }

  void run() {
    io_service_.run();
  }

private:
  void do_accept() {
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
                               auto conn = std::make_shared<connection<Handler, connection_manager<Handler>>>(std::move(socket_),
                                                                                                           app_, &connection_manager_);
                               connection_manager_.start(conn);
                             }

                             do_accept();
                           });
  }

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;

  // The connection manager which owns all live connections.
  //connection_manager connection_manager_;

  Handler *app_;
  connection_manager<Handler> connection_manager_;
};

} // namespace zion

#endif //ZION_SERVER_H
