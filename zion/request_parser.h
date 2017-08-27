//
// Created by Shihao Jing on 6/19/17.
//

#ifndef ZION_REQUEST_PARSER_H
#define ZION_REQUEST_PARSER_H

#include "request.h"
#include "http_parser.h"

namespace zion {

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
      req->method_code = parser->method;
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
    return 0;
  }

  bool parse(request &req, const char* buffer, size_t length) {
    http_parser_settings settings;
    settings.on_message_begin = on_message_begin;
    settings.on_message_complete = on_message_complete;
    settings.on_url = on_url;
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_headers_complete = on_headers_complete;
    settings.on_body = on_body;

    http_parser *parser = new http_parser();
    http_parser_init(parser, HTTP_REQUEST); /* initialise parser */
    parser->data = &req;

    size_t nparsed = http_parser_execute(parser, &settings, buffer, length);
    return nparsed == length;
  }
};

} //namespace zion

#endif //ZION_REQUEST_PARSER_H
