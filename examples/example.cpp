//
// Created by Shihao Jing on 6/25/17.
//

#include "zion.h"

int main() {
  zion::Zion app("127.0.0.1", "8080", "./");
  app.route("/")
  ([]() {   // resp should be response type
    return "hello, world";
  });

  app.run();
}