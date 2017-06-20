//
// Created by Shihao Jing on 6/13/17.
//

#ifndef SLASH_CONNECTION_H
#define SLASH_CONNECTION_H

#include <boost/asio.hpp>
#include <memory>
#include "request.h"
#include "request_parser.h"

namespace HTTP {
namespace Server {

class connection_manager;

class connection : public std::enable_shared_from_this<connection>
{
public:
  connection(const connection&) = delete;
  connection& operator=(const connection&) = delete;

  // Construct a connection with the given socket.
  explicit connection(boost::asio::ip::tcp::socket socket, connection_manager &manager);

  // Start the asynchronous operation for the connection
  void start();

  // Stop all asynchronous operation associated with the connection
  void stop();

private:
  // Perform an asynchronous read operation.
  void do_read();

  // Perform an asynchronous write operation.
  void do_write();

  // Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  // Buffer for incoming data.
  std::array<char, 8192> buffer_;

  // The manager for this connection.
  connection_manager& connection_manager_;

  // incoming request
  request request_;

  request_parser request_parser_;
};

} // namespace Server
} // namespace HTTP


#endif //SLASH_CONNECTION_H
