//
// Created by Shihao Jing on 6/25/17.
//

#include "app.h"
#include <iostream>

int main() {
  zion::Zion app;
  app.route("/", [](const zion::request &req, std::string &resp) {
    std::cout << "request url: /" << std::endl;
  });
  app.route("/id", [](const zion::request &req, std::string &resp) {
    std::cout << "request url: /id" << std::endl;
  });

  zion::request req;
  req.uri = "/";
  std::string resp = "";
  app.handle(req, resp);
  req.uri = "/id";
  app.handle(req, resp);
}