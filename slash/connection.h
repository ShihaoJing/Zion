//
// Created by Shihao Jing on 6/13/17.
//

#ifndef ZION_CONNECTION_H
#define ZION_CONNECTION_H

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include "response.h"
#include "request.h"
#include "request_parser.h"

namespace zion {

template <typename Handler>
class connection : public std::enable_shared_from_this<connection<Handler>>
{
public:
  connection(const connection&) = delete;
  connection& operator=(const connection&) = delete;

  // Construct a connection with the given socket.
  explicit connection(boost::asio::ip::tcp::socket socket,
                      Handler *app)
      : socket_(std::move(socket)),
        app_(app)
  {
  }

  // Start the asynchronous operation for the connection
  void start() {
    do_read();
  }

  // Stop all asynchronous operation associated with the connection
  void stop() {
    socket_.close();
  }

private:
  // Perform an asynchronous read operation.
  void do_read() {
    auto self = this->shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_),
                            [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
                            {
                              std::cout << "start read" << std::endl;
                              if (!ec) {
                                request_parser::result_type result;
                                request_parser_.parse(request_, buffer_.data(), bytes_transferred);
                                std::cout << request_.uri << std::endl;
                                std::cout << request_.method << std::endl;
                                std::cout << request_.http_version_major << "." << request_.http_version_minor << std::endl;
                                for (auto header : request_.headers) {
                                  std::cout << header.first << " : " << header.second << std::endl;
                                }

                                handle();
                                app_->handle(request_, response_);
                                complete();

                              }
                              else if (ec != boost::asio::error::operation_aborted) {
                                //connection_manager_->stop(self);
                              }
                            });
  }

  // put response into buffer
  void complete() {

  }


  // handle request header
  void handle() {

  }

  // Perform an asynchronous write operation.
  void do_write() {
    //TODO: send response
  }

  // Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  // Buffer for incoming data.
  std::array<char, 8192> buffer_;

  // The manager for this connection.
 // connection_manager *connection_manager_;

  response response_;
  // Incoming request
  request request_;

  request_parser request_parser_;

  Handler *app_;
};

} // namespace zion


#endif //ZION_CONNECTION_H
