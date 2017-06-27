//
// Created by Shihao Jing on 6/25/17.
//

#ifndef ZION_ROUTING_H
#define ZION_ROUTING_H

#include <unordered_map>
#include <string>
#include "request.h"
#include "response.h"

namespace zion {

class Rule
{
public:
  Rule(std::string rule) : rule_(rule){

  }

  template <typename Func>
  void operator() (Func f) {
    handler_ = [f] {
      return response(f());
    };
  }

  bool match (const request &req) {
    return req.uri == rule_;
  }

  response handle() {
    return handler_();
  }

private:
  std::string rule_;
  std::string name_;
  std::function<response()> handler_;
};

class Router
{
public:
  Router()
  {
  }

  Rule& new_rule(std::string &rule) {
    rules_.push_back(Rule(rule));
    return rules_.back();
  }

  response handle(const request &req)
  {
    for (auto rule: rules_) {
      if (rule.match(req)) {
        return rule.handle();
      }
    }

    return response(response::not_found);
  }

private:
  std::vector<Rule> rules_;
};

}

#endif //ZION_ROUTING_H
