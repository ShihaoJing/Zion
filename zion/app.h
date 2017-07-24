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

#define ZION_ROUTE(app, url) app.route<zion::util::get_parameter_tag(url)>(url)

namespace zion {

class Zion
{
public:
  typedef Server<Zion> server_t;

  Zion() = default;

  Zion& port(std::string port) {
    port_ = port;
    return *this;
  }

  Zion& bindaddr(std::string bindaddr) {
    bindaddr_ = bindaddr;
    return *this;
  }

  template <uint64_t Tag>
  auto route(std::string rule)
    -> typename std::result_of<decltype(&Router::new_param_rule<Tag>)(Router, std::string&)>::type
  {
    return router_.new_param_rule<Tag>(rule);
  }

  Rule& route(std::string rule)
  {
    return router_.new_rule(rule);
  }

  response handle(const request &req) {
    return router_.handle(req);
  }

  void run() {
    server_ = std::move(std::unique_ptr<server_t>(new server_t(bindaddr_, port_, doc_root_, this)));
    server_->run();
  }

private:
  std::string port_ = "80";
  std::string bindaddr_ = "0.0.0.0";
  std::string doc_root_ = "/var/www/html";
  std::unique_ptr<server_t> server_;
  Router router_;
};

} //namespace zion

#endif //ZION_APP_H
