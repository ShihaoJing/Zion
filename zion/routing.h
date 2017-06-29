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

class Trie
{
  struct TrieNode
  {
    TrieNode *children[ALPHABET_SIZE] = {};
    TrieNode *param_children[PARAMTYPE_NUM] = {};

    Rule *rule = nullptr;
    bool isLeaf = false;
  };

  enum class ParamType
  {
    INT
  };

  struct ParamTraits
  {
    ParamType type;
    std::string name;
  };

public:
  Trie() : root_(new TrieNode)
  {
  }

  Rule& insert(std::string key) {
    TrieNode *cur = root_;
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
    cur->rule = new Rule(key);
    cur->isLeaf = true;
    return *(cur->rule);
  }

  Rule* search(std::string key) {
    TrieNode *cur = root_;
    for (size_t i = 0; i < key.length(); /* */) {
      char c = key[i];
      if (c == '/') {
        int index = key[i] - '!';
        if (!cur->children[index])
          return nullptr;
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
            return nullptr;
          cur = cur->children[index];
          ++i;
        }
      }
    }
    if (cur == nullptr || cur->rule == nullptr)
      return nullptr;
    else return cur->rule;
  }

private:
  TrieNode *root_;
};

class Router
{
public:
  Router() : trie_()
  {
  }

  Rule& new_rule(std::string &rule) {
    return trie_.insert(rule);
  }

  response handle(const request &req)
  {
    Rule *rule = trie_.search(req.uri);
    if (rule)
      return rule->handle();

    return response(response::not_found);
  }

private:
  Trie trie_;
};

}

#endif //ZION_ROUTING_H
