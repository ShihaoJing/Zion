//
// Created by Shihao Jing on 6/25/17.
//

#ifndef SLASH_ROUTING_H
#define SLASH_ROUTING_H

#include <unordered_map>
#include <string>
#include "request.h"

namespace zion {

class Router
{
public:
  typedef void (*RequestHandler)(const request &req, std::string &res);
  Router()
  {
  }

  template <typename RequestHandler>
  void add(std::string url, RequestHandler &func) {
    map_.emplace(url, func);
  }

  void handle(const request &req, std::string &res)
  {
    auto iter = map_.find(req.uri);
    if (iter == map_.end())
    {
      // not found
    }

    (*iter->second)(req, res);
  }

private:
  std::unordered_map<std::string, RequestHandler> map_;
};

}

#endif //SLASH_ROUTING_H
