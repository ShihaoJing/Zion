//
// Created by Shihao Jing on 6/25/17.
//

#include "zion.h"
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace zion;

int main() {


  zion::Zion app;

  ZION_ROUTE(app, "/name/<string>/id/<int>/weight/<float>")
      ([](std::string name, int id, float_t weight) {   // resp should be response type
        std::ostringstream os;
        os << "name is " << name << " id is " << id << " and weight is " << weight;
        return zion::response(os.str());
      });

  ZION_ROUTE(app, "/weight/<float>")
      ([](float_t weight) {   // resp should be response type
        std::ostringstream os;
        os << "weight is " << weight;
        return zion::response(os.str());
      });

  ZION_ROUTE(app, "/index")
      ([](const zion::request &req){
        Document d;
        d.Parse(req.body.data());
        // 2. Modify it by DOM.
        Value& s = d["stars"];
        s.SetInt(s.GetInt() + 1);

        // 3. Stringify the DOM
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
      });

  app.port("8080")
      .bindaddr("127.0.0.1")
      .run();
}