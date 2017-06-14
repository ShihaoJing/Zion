//
// Created by Shihao Jing on 6/13/17.
//

#include "Connection.h"
namespace HTTP {
namespace Server {

Connection::Connection(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket))
{
}

void Connection::start() {
  do_read();
}

void Connection::stop() {
  socket_.close();
}

void Connection::do_read() {

}


} // namespace HTTP
} // namespace Server