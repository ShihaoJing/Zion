//
// Created by Shihao Jing on 6/27/17.
//

#include "gtest/gtest.h"
#include "zion.h"

using namespace zion;

TEST(Routing, SimplePath) {
  Zion app;
  app.route("/")
      ([]() {   // resp should be response type
        return "hello, world";
      });
  app.route("/hello")
      ([]() {   // resp should be response type
        return "hello";
      });

  {
    request req;
    req.uri = "/";
    response res = app.handle(req);
    ASSERT_EQ(static_cast<int>(response::ok), static_cast<int>(res.status_));
  }

  {
    request req;
    req.uri = "/hello";
    response res = app.handle(req);
    ASSERT_EQ(static_cast<int>(response::ok), static_cast<int>(res.status_));
  }

  {
    request req;
    req.uri = "/posts";
    response res = app.handle(req);
    ASSERT_EQ(static_cast<int>(response::not_found), static_cast<int>(res.status_));
  }
}