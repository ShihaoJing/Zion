//
// Created by Shihao Jing on 6/25/17.
//

#ifndef ZION_ROUTING_H
#define ZION_ROUTING_H

#include <unordered_map>
#include <string>
#include "request.h"
#include "response.h"

#define ALPHABET_SIZE 128
#define PARAMTYPE_NUM  6

namespace zion {

enum class ParamType
{
  INT
};

struct ParamTraits
{
  ParamType type;
  std::string name;
};

class Rule;

struct TrieNode
{
  TrieNode *children[ALPHABET_SIZE] = {};
  TrieNode *param_children[PARAMTYPE_NUM] = {};

  std::unique_ptr<Rule> rule;
  bool isLeaf = false;
};

void insert(TrieNode *root, std::string key) {
  TrieNode *cur = root;
  for (int i = 0; i < key.length(); ++i) {
    char c = key[i];
    if (c == '<') {
      bool found = false;

      static ParamTraits paramTraits[] =
          {
              { ParamType::INT, "<int>" },
          };

      for(auto it = std::begin(paramTraits); it != std::end(paramTraits); ++it)
      {
        if (key.compare(i, it->name.size(), it->name) == 0)
        {
          if (!cur->param_children[(int)it->type]) {
            cur->param_children[(int)it->type] = new TrieNode;
          }
          cur = cur->param_children[(int)it->type];
          i += it->name.size() - 1; // for loop will increment i by 1
          found = true;
          break;
        }
      }
      if (!found) {
        throw std::runtime_error("Invalid parameter type" + key);
      }
    }
    else {
      int index = key[i] - '!';
      if (!cur->children[index]) {
        cur->children[index] = new TrieNode;
      }
      cur = cur->children[index];
    }
  }
  if (cur != root) cur->isLeaf = true;
}

bool search(TrieNode *root, std::string key) {
  TrieNode *cur = root;
  for (size_t i = 0; i < key.length(); /* */) {
    char c = key[i];
    if (c == '/') {
      int index = key[i] - '!';
      if (!cur->children[index])
        return false;
      cur = cur->children[index];
      ++i;
    }
    else {
      // match <int> pattern
      if (cur->param_children[0]) {
        long long value = 0;
        int type = 1;
        int j = i;
        while (j < key.length()) {
          if ( j != i && (key[j] < '0' || key[j] > '9'))
            break;
          if (key[j] == '-') type = -1;
          value = value * 10 + key[j] - '0';
          ++j;
        }
        if (j == key.length() || key[j] == '/') {
          i = j;
          cur = cur->param_children[0];
          continue;
        }
      }

      // no param pattern match, do normal path matching
      while (i < key.length() && key[i] != '/') {
        int index = key[i] - '!';
        if (!cur->children[index])
          return false;
        cur = cur->children[index];
        ++i;
      }
    }
  }
  return cur != nullptr && cur->isLeaf;
}

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
