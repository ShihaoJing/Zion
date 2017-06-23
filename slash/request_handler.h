//
// Created by fanshiliang on 2017/6/22.
//
/*
 * Temporarily only support return specific files in response
 *
 * Restful service handler to be added
 */
#ifndef SLASH_REQUEST_HANDLER_H
#define SLASH_REQUEST_HANDLER_H

#include <string>
#include "request.h"
#include "response.h"
#include "mime.h"

namespace HTTP {
namespace Server {



// The common handler for all incoming requests
class request_handler
{
public:
    request_handler(const request_handler& ) = delete;
    request_handler& operator=(const request_handler& ) = delete;

    // construct with a directory containing files to be served
    explicit request_handler(const std::string& doc_root);

    // handle a request and produce a response

    // Reply class to be developed
    //void handle_request(const request& req, response& rep);

    void handle_request(const request& req, response& rep);

private:
    // the directory containing the files to be served.
    std::string doc_root_;

    //perform URL-decoding on a string. returns false if the encoding was invalid
    static bool url_decode(const std::string& in, std::string& out);
};

}//namespace server
}//namespace http

#endif //SLASH_REQUEST_HANDLER_H

