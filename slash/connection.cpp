//
// Created by Shihao Jing on 6/13/17.
//

#include "connection.h"
#include "connection_manager.h"
#include <iostream>

namespace HTTP {
    namespace Server {

        connection::connection(boost::asio::ip::tcp::socket socket, connection_manager &manager,
                               request_handler &handler)
                : socket_(std::move(socket)),
                  connection_manager_(manager),
                  request_handler_(handler) {
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
                                    [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                                        if (!ec) {
                                            request_parser::result_type result;
                                            //  Still have some concerns. the use of result_type. below only have two
                                            //  result type. In request parser, result_type have three value: good, bad,
                                            //  intermidate.
                                            if (request_parser_.parse(request_, buffer_.data(), bytes_transferred))
                                            {
                                                request_handler_.handle_request(request_, response_);
                                                do_write();
                                            }
                                            // request_handler here
                                            // good request
                                            else
                                            {
                                                response_ = response::stock_reply(response::bad_request);
                                                do_write();
                                            }

                                        } else if (ec != boost::asio::error::operation_aborted) {
                                            self->connection_manager_.stop(self);
                                        }
                                    });
        }

        void connection::do_write() {
            //TODO: send response
            auto self(shared_from_this());
            boost::asio::async_write(socket_, response_.to_buffers(),
                [this, self](boost::system::error_code ec, std::size_t)
                {
                    if (!ec)
                    {
                        boost::system::error_code ignored_ec;
                        //shutdown send and receive operation
                        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                    }

                    if (ec != boost::asio::error::operation_aborted)
                    {
                        connection_manager_.stop(shared_from_this());
                    }
                });
        }


    } // namespace HTTP
} // namespace Server