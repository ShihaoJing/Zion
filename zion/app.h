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

  Rule& route(std::string rule)
  {
    return router_.new_rule(rule);
  }

  response handle(const request &req) {
    return router_.handle(req);
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
