//
// Created by Shihao Jing on 6/25/17.
//

#ifndef SLASH_APP_H
#define SLASH_APP_H

#include "routing.h"
#include "request.h"
#include <string>

namespace zion {

class Zion
{
public:
  Zion()
  {
  }

  void route(std::string url, Router::RequestHandler func) {
    router_.add(url, func);
  }

  void handle(const request &req, std::string &res)
  {
    router_.handle(req, res);
  }

private:
  int port_ = 8080;
  //std::unique_ptr<HTTP::Server::Server> server_;
  Router router_;
};

}

#endif //SLASH_APP_H
