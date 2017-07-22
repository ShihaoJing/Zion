//
// Created by Shihao Jing on 6/27/17.
//

#include "gtest/gtest.h"
#include "zion.h"

using namespace zion;
using namespace std;

TEST(Routing, SimplePath) {
  Zion app;
  ZION_ROUTE(app, "/<int>")
      ([]() {   // resp should be response type
        return "hello, world";
      });
  ZION_ROUTE(app, "/hello/<int>")
      ([]() {   // resp should be response type
        return "hello";
      });

  {
    request req;
    req.uri = "/";
    response res = app.handle(req);
    EXPECT_EQ(static_cast<int>(response::ok), static_cast<int>(res.status_));
  }

  {
    request req;
    req.uri = "/hello";
    response res = app.handle(req);
    EXPECT_EQ(static_cast<int>(response::ok), static_cast<int>(res.status_));
  }

  {
    request req;
    req.uri = "/posts";
    response res = app.handle(req);
    EXPECT_EQ(static_cast<int>(response::not_found), static_cast<int>(res.status_));
  }
}

TEST(Routing, Trie) {
  /*{
    Trie *trie = new Trie;

    vector<string> keys = {"/", "/hello", "/id/<int>"};
    for (auto key : keys) {
      trie->insert(key);
    }

    util::routing_param routing_params;
    EXPECT_TRUE(trie->search("/", routing_params));
    EXPECT_TRUE(trie->search("/hello", routing_params));
    EXPECT_TRUE(trie->search("/id/123", routing_params));
    EXPECT_EQ(123, routing_params.int_params.back());
    EXPECT_TRUE(trie->search("/id/+123", routing_params));
    EXPECT_EQ(123, routing_params.int_params.back());
    EXPECT_TRUE(trie->search("/id/-123", routing_params));
    EXPECT_EQ(-123, routing_params.int_params.back());
    EXPECT_FALSE(trie->search("/hello/12a", routing_params));
    EXPECT_EQ(-123, routing_params.int_params.back());
  }

  {
    Trie *trie = new Trie;

    vector<string> keys = {"/hello/", "/id/<int>/"};
    for (auto key : keys) {
      trie->insert(key);
    }

    util::routing_param routing_params;
    EXPECT_TRUE(trie->search("/id/123/", routing_params));
    // below examples need to be redirected with trailing slash to make TRUE
    EXPECT_FALSE(trie->search("/hello", routing_params));
    EXPECT_FALSE(trie->search("/id/123", routing_params));
    EXPECT_FALSE(trie->search("/id/+123", routing_params));
    EXPECT_FALSE(trie->search("/id/-123", routing_params));
  }*/
}

TEST(Routing, Utility) {
  EXPECT_TRUE(util::is_int(util::StrWrap("<int>"), 0));
  EXPECT_FALSE(util::is_int(util::StrWrap("<123>"), 0));
  EXPECT_EQ(util::get_parameter_tag("<int>/<int>"), 7);
}