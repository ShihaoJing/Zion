//
// Created by Shihao Jing on 6/19/17.
//

#include "request_parser.h"
#include "http_parser.c"

namespace HTTP {
namespace Server {

bool request_parser::parse(request &req, const char *buffer, size_t length) {
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

}
}