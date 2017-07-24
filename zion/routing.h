//
// Created by Shihao Jing on 6/25/17.
//

#ifndef ZION_ROUTING_H
#define ZION_ROUTING_H

#include <unordered_map>
#include <string>
#include <memory>
#include "request.h"
#include "response.h"
#include "utility.h"

#define ALPHABET_SIZE 128
#define PARAMTYPE_NUM  6

namespace zion {

class BaseRule
{
public:
  BaseRule(std::string rule)
      : rule_(rule)
  {
  }

  virtual ~BaseRule()
  {
  }

  BaseRule& name(std::string name) {
    name_ = name;
    return *this;
  }

  //virtual void validate();

  virtual response handle(const request&, const util::routing_param&)
  {
    return response(response::not_found);
  }

  std::string rule_;
  std::string name_;
};

class Rule : public BaseRule
{
public:
  Rule(std::string rule)
      : BaseRule(rule)
  {
  }

  Rule& name(std::string name) {
    name_ = name;
    return *this;
  }

  template <typename Func>
  void operator() (Func f) {
    handler_ = [f] {
      return response(f());
    };
  }

  template <typename Func>
  void operator() (std::string name, Func f) {
    name_ = name;
    handler_ = [f] {
      return response(f());
    };
  }

  response handle(const request&, const util::routing_param&) {
    return handler_();
  }

private:
  std::function<response()> handler_;
};


template <typename ... Args>
class ParamRule : public BaseRule
{
private:
  template <typename T, int Pos>
  struct call_pair
  {
    using type = T;
    static const int pos = Pos;
  };

  template <typename F, int NInt, int NUint, int NDouble, int NString, typename S1, typename S2> struct call
  {
  };

  template <typename F, int NInt, int NUint, int NDouble, int NString, typename ... Args1, typename ... Args2>
  struct call<F, NInt, NUint, NDouble, NString, util::S<int64_t, Args1...>, util::S<Args2...>>
  {
    response operator()(F& handler, const util::routing_param& params)
    {
      using pushed = typename util::S<Args2...>::template push_back<call_pair<int64_t, NInt>>;
      return call<F, NInt+1, NUint, NDouble, NString,
                  util::S<Args1...>, pushed>()(handler, params);
    }
  };

  template <typename F, int NInt, int NUint, int NDouble, int NString, typename ... Args1>
  struct call<F, NInt, NUint, NDouble, NString, util::S<>, util::S<Args1...>>
  {
    response operator()(F& handler, const util::routing_param& params)
    {
      return handler(
          params.get<typename Args1::type>(Args1::pos)...
      );
      //return response(500);
    }
  };
public:
  ParamRule(std::string rule) : BaseRule(rule){

  }

  template <typename Func>
  void operator() (Func f) {
    handler_ = [f](Args ... args) {
      return response(f(args...));
    };
  }

  bool match (const request &req) {
    return req.uri == rule_;
  }

  response handle(const request&, const util::routing_param &params) {
    return call<decltype(handler_), 0, 0, 0, 0, util::S<Args...>, util::S<>>()(handler_, params);
  }

private:
  std::function<response(Args...)> handler_;
};

class Trie
{
  struct TrieNode
  {
    TrieNode *children[ALPHABET_SIZE] = {};
    TrieNode *param_children[PARAMTYPE_NUM] = {};

    int rule_index = -1;
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

   void insert(std::string key, int rule_index) {
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
        int index = key[i];
        if (!cur->children[index]) {
          cur->children[index] = new TrieNode;
        }
        cur = cur->children[index];
      }
    }
    cur->rule_index = rule_index;
  }

  int search(std::string key, util::routing_param &routing_params) {
    TrieNode *cur = root_;
    for (size_t i = 0; i < key.length(); /* */) {
      char c = key[i];
      if (c == '/') {
        int index = key[i];
        if (!cur->children[index])
          return -1;
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
            if (key[j] != '+' && key[j] != '-') value = value * 10 + key[j] - '0';
            ++j;
          }
          if (j == key.length() || key[j] == '/') {
            routing_params.int_params.push_back(value * type);
            i = j;
            cur = cur->param_children[0];
            continue;
          }
        }

        // no param pattern match, do normal path matching
        while (i < key.length() && key[i] != '/') {
          int index = key[i];
          if (!cur->children[index])
            return -1;
          cur = cur->children[index];
          ++i;
        }
      }
    }

    if (cur == nullptr || cur->rule_index == -1)
      return -1;

    return cur->rule_index;
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

  template <uint64_t N>
  typename util::arguments<N>::type::template rebind<ParamRule>& new_param_rule(std::string rule) {
    using RuleT = typename util::arguments<N>::type::template rebind<ParamRule>;
    auto ruleObject = new RuleT(rule);
    rules_.emplace_back(ruleObject);
    trie_.insert(rule, rules_.size() - 1);
    return *ruleObject;
  }

  Rule& new_rule(std::string rule) {
    Rule *r(new Rule(rule));
    rules_.emplace_back(r);
    trie_.insert(rule, rules_.size() - 1);
    return *r;
  }

  response handle(const request &req)
  {
    util::routing_param routing_params;
    int rule_index = trie_.search(req.uri, routing_params);
    if (rule_index != -1)
      return rules_[rule_index]->handle(req, routing_params);

    return response(response::not_found);
  }

private:
  std::vector<std::unique_ptr<BaseRule>> rules_;
  Trie trie_;
};

}

#endif //ZION_ROUTING_H
