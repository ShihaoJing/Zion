//
// Created by Shihao Jing on 6/25/17.
//

#include "app.h"
#include <iostream>

int main() {
  zion::Zion app("127.0.0.1", "8080", "./");
  app.route("/", [](const zion::request &req, zion::response &resp) {   // resp should be response type
    std::cout << "request url: /" << std::endl;
  });
  app.route("/id", [](const zion::request &req, zion::response &resp) {
    std::cout << "request url: /id" << std::endl;
  });

  app.run();
}