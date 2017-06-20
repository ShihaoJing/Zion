//
// Created by Shihao Jing on 6/13/17.
//

#include "connection.h"
#include "connection_manager.h"
#include <iostream>
namespace HTTP {
namespace Server {

connection::connection(boost::asio::ip::tcp::socket socket, connection_manager &manager)
    : socket_(std::move(socket)),
      connection_manager_(manager)
{
}

void connection::start() {
  do_read();
}

void connection::stop() {
  socket_.close();
}

void connection::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
                          [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
                          {
                            if (!ec) {
                              request_parser::result_type result;
                              request_parser_.parse(request_, buffer_.data(), bytes_transferred);
                              std::cout << request_.uri << std::endl;
                              std::cout << request_.method << std::endl;
                              std::cout << request_.http_version_major << "." << request_.http_version_minor << std::endl;
                              for (auto header : request_.headers) {
                                std::cout << header.first << " : " << header.second << std::endl;
                              }
                              //TODO: handle request
                            }
                            else if (ec != boost::asio::error::operation_aborted) {
                              self->connection_manager_.stop(self);
                            }
                          });
}

void connection::do_write() {
  //TODO: send response
}


} // namespace HTTP
} // namespace Server