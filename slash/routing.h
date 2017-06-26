//
// Created by Shihao Jing on 6/25/17.
//

#ifndef ZION_ROUTING_H
#define ZION_ROUTING_H

#include <unordered_map>
#include <string>
#include "request.h"
#include "response.h"

namespace zion {
typedef void (*RequestHandler)(const request &req, response &res);

class Router
{
public:
  Router()
  {
  }

  void add(std::string url, RequestHandler &func) {
    map_.emplace(url, func);
  }

  void handle(const request &req, response &res)
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

#endif //ZION_ROUTING_H
