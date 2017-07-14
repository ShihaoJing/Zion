//
// Created by Shihao Jing on 7/13/17.
//

#ifndef ZION_UTILITY_H
#define ZION_UTILITY_H

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


} // namespace util
} // namespace zion

#endif //ZION_UTILITY_H
