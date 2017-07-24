//
// Created by Shihao Jing on 6/25/17.
//

#include "zion.h"
#include <sstream>

int main() {
  zion::Zion app;

  ZION_ROUTE(app, "/id/<int>")
      ([](int id) {   // resp should be response type
        std::ostringstream os;
        os << "id is " << id;
        return zion::response(os.str());
      });

  app.route("/index")
      ([](){
        return "hello world";
      });

  app.port("8080")
      .bindaddr("127.0.0.1")
      .run();
}