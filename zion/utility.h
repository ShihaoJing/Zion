//
// Created by Shihao Jing on 7/13/17.
//

#ifndef ZION_UTILITY_H
#define ZION_UTILITY_H

#include <vector>

namespace zion {
namespace util {

struct OutOfRange
{
  OutOfRange(unsigned pos, unsigned length) {}
};

constexpr unsigned requires_inRange(unsigned i, unsigned len) {
  return i >= len ? throw OutOfRange(i, len) : i;
}

class StrWrap
{
public:
  template< unsigned N >
  constexpr StrWrap(const char(&arr)[N]) : begin_(arr), size_(N - 1) {
    static_assert( N >= 1, "not a string literal");
  }

  constexpr char operator[]( unsigned i ) const {
    return requires_inRange(i, size_), begin_[i];
  }

  constexpr operator const char *() const {
    return begin_;
  }

  constexpr unsigned size() const {
    return size_;
  }

private:
  const char * const begin_;
  unsigned size_;
};

constexpr unsigned find_closing_tag(StrWrap s, unsigned p) {
  return s[p] == '>' ? p : find_closing_tag(s, p+1);
}

constexpr bool is_equ_n(StrWrap a, unsigned i, StrWrap b, unsigned j, unsigned n) {
  return
      i + n > a.size() || j + n > b.size() ? false :
      n == 0 ? true :
      a[i] != b[j] ? false :
      is_equ_n(a, i + 1, b, j + 1, n - 1);
}

constexpr bool is_int(StrWrap s, unsigned i) {
  return is_equ_n(s, i, "<int>", 0, 5);
}

constexpr uint64_t get_parameter_tag(StrWrap s, unsigned p = 0) {
  return
    p == s.size() ? 0 :
    s[p] == '<' ?
        (is_int(s, p) ? get_parameter_tag(s, find_closing_tag(s, p)) * 6 + 1
         : throw std::runtime_error("invalid parameter type") ) :
    get_parameter_tag(s, p+1);
}

template <typename ... T>
struct S
{
  template <typename U>
  using push = S<U, T...>;
  template <typename U>
  using push_back = S<T..., U>;
  template <template<typename ... Args> class U>
  using rebind = U<T...>;
};

template <int N>
struct single_tag_to_type
{
};

template <>
struct single_tag_to_type<1>
{
  using type = int64_t;
};

template <uint64_t Tag>
struct arguments
{
  using subarguments = typename arguments<Tag/6>::type;
  using type =
  typename subarguments::template push<typename single_tag_to_type<Tag%6>::type>;
};

template <>
struct arguments<0>
{
  using type = S<>;
};

struct routing_param
{
  std::vector<int64_t> int_params;
  std::vector<uint64_t> uint_params;
  std::vector<double> double_params;
  std::vector<std::string> string_params;

  template <typename T>
  T get(unsigned) const;

};

template<>
int64_t routing_param::get<int64_t>(unsigned index) const
{
  return int_params.at(index);
}

} // namespace util
} // namespace zion

#endif //ZION_UTILITY_H
