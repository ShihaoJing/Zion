//
// Created by Shihao Jing on 6/25/17.
//

#include "zion.h"
#include <sstream>

int main() {
  zion::Zion app;

  ZION_ROUTE(app, "/id/<int>/weight/<float>")
      ([](int id, float_t weight) {   // resp should be response type
        std::ostringstream os;
        os << "id is " << id << " and weight is " << weight;
        return zion::response(os.str());
      });

  ZION_ROUTE(app, "/weight/<float>")
      ([](float_t weight) {   // resp should be response type
        std::ostringstream os;
        os << "weight is " << weight;
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