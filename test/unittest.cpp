//
// Created by Shihao Jing on 6/27/17.
//

#include "gtest/gtest.h"
#include "zion.h"
#include <string>
#include <vector>

using namespace zion;
using namespace std;

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
  {
    Trie *trie = new Trie;

    vector<string> keys = {"/", "/hello", "/id/<int>"};
    for (auto key : keys) {
      trie->insert(key);
    }

    EXPECT_TRUE(trie->search("/"));
    EXPECT_TRUE(trie->search("/hello"));
    EXPECT_TRUE(trie->search("/id/123"));
    EXPECT_TRUE(trie->search("/id/+123"));
    EXPECT_TRUE(trie->search("/id/-123"));
    EXPECT_FALSE(trie->search("/hello/12a"));
  }

  {
    Trie *trie = new Trie;

    vector<string> keys = {"/hello/", "/id/<int>/"};
    for (auto key : keys) {
      trie->insert(key);
    }

    EXPECT_TRUE(trie->search("/id/123/"));
    // below examples need to be redirected with trailing slash to make TRUE
    EXPECT_FALSE(trie->search("/hello"));
    EXPECT_FALSE(trie->search("/id/123"));
    EXPECT_FALSE(trie->search("/id/+123"));
    EXPECT_FALSE(trie->search("/id/-123"));
  }
}