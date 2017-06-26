//
// Created by Shihao Jing on 6/25/17.
//

#ifndef ZION_APP_H
#define ZION_APP_H

#include "routing.h"
#include "request.h"
#include "server.h"
#include <memory>
#include <string>

namespace zion {

class Zion
{
public:
  Zion(const std::string &address, const std::string &port, const std::string &doc_root)
      : server_(address, port, doc_root, this)
  {
  }

  void route(std::string url, RequestHandler func) {
    router_.add(url, func);
  }

  void handle(const request &req, response &res) {
    router_.handle(req, res);
  }

  void run() {
    server_.run();
  }

private:
  Server<Zion> server_;
  Router router_;
};

} //namespace zion

#endif //ZION_APP_H
