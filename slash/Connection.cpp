//
// Created by Shihao Jing on 6/13/17.
//

#include "Connection.h"
#include "ConnectionManager.h"
#include <iostream>
namespace HTTP {
namespace Server {

Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager &manager)
    : socket_(std::move(socket)),
      connection_manager_(manager)
{
}

void Connection::start() {
  do_read();
}

void Connection::stop() {
  socket_.close();
}

void Connection::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
                          [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
                          {
                            if (!ec) {
                              for (auto c: buffer_) std::cout << c;
                              //TODO: request parsing
                            }
                            else if (ec != boost::asio::error::operation_aborted) {
                              connection_manager_.stop(self);
                            }
                          });
}

void Connection::do_write() {
  //TODO: send response
}


} // namespace HTTP
} // namespace Server