//
// Created by Shihao Jing on 6/19/17.
//

#ifndef SLASH_REQUEST_H
#define SLASH_REQUEST_H

#include <vector>
#include <unordered_map>
#include <string>

namespace HTTP {
namespace Server {

struct request
{
  int http_version_major;
  int http_version_minor;
  std::string method;
  std::string uri;
  std::string header_field;
  std::string header_value;
  std::string body;
  std::unordered_map<std::string, std::string> headers;
  int header_building_state = 0;
};


} // namespace HTTP
} // namespace Server
#endif //SLASH_REQUEST_H