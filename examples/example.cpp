//
// Created by Shihao Jing on 6/25/17.
//

#include "zion.h"

int main() {
  zion::Zion app;

  app.route("/")
  ([]() {   // resp should be response type
    return "hello, world";
  });

  app.port("8080")
      .bindaddr("127.0.0.1")
      .run();
}