//
// Created by Shihao Jing on 6/19/17.
//

#ifndef SLASH_REQUEST_PARSER_H
#define SLASH_REQUEST_PARSER_H

#include "request.h"
#include "http_parser.h"

namespace HTTP {
namespace Server {

class request_parser
{
public:
  request_parser() = default;

  enum result_type { good, bad, indeterminate };

  static int on_message_begin(http_parser* parser)
  {
    return 0;
  }

  static int on_url(http_parser* parser, const char* at, size_t length)
  {
    request *req = static_cast<request*>(parser->data);
    req->uri.append(at, length);
    return 0;
  }

  static int on_header_field(http_parser* parser, const char* at, size_t length)
  {
    request *req = static_cast<request*>(parser->data);
    switch (req->header_building_state)
    {
      case 0:
        if (!req->header_value.empty())
        {
          req->headers.emplace(std::move(req->header_field), std::move(req->header_value));
        }
        req->header_field.assign(at, length);
        req->header_building_state = 1;
        break;
      case 1:
        req->header_field.append(at, length);
        break;
      default:
        break;
    }
    return 0;
  }

  static int on_header_value(http_parser* parser, const char* at, size_t length)
  {
    request *req = static_cast<request*>(parser->data);
    switch (req->header_building_state)
    {
      case 0:
        req->header_value.append(at, length);
        break;
      case 1:
        req->header_building_state = 0;
        req->header_value.assign(at, length);
        break;
    }
    return 0;
  }

  static int on_headers_complete(http_parser* parser)
  {
    request *req = static_cast<request*>(parser->data);
    if (!req->header_field.empty())
    {
      req->headers.emplace(std::move(req->header_field), std::move(req->header_value));
      req->method = http_method_str(http_method(parser->method));
      req->http_version_major = parser->http_major;
      req->http_version_minor = parser->http_minor;
    }
    return 0;
  }

  static int on_body(http_parser* parser, const char* at, size_t length)
  {
    request *req = static_cast<request*>(parser->data);
    req->body.append(at, length);
    return 0;
  }

  static int on_message_complete(http_parser* parser)
  {
    request *req = static_cast<request*>(parser->data);
    // TODO: parse url params
    return 0;
  }

  bool parse(request &req, const char* buffer, size_t length);
};

}
}

#endif //SLASH_REQUEST_PARSER_H
