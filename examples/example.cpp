//
// Created by Shihao Jing on 6/25/17.
//

#include "zion.h"
#include <sstream>

int main() {
  zion::Zion app;

  ZION_ROUTE(app, "/<int>")
  ([](int count) {   // resp should be response type
    if (count > 100)
      return zion::response(zion::response::not_found);
    std::ostringstream os;
    os << count << " bottles of beer!";
    return zion::response(os.str());
  });

  app.port("8080")
      .bindaddr("127.0.0.1")
      .run();
}