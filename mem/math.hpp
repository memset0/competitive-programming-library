#ifndef MEM_MATH_H
#define MEM_MATH_H

#include <algorithm>

using std::max;
using std::min;

template <typename T> inline T abs(T x) { return x < 0 ? -x : x; }
template <typename T> inline T lcm(T n, T m) { return n / gcd(n, m) * m; }
template <typename T> inline T gcd(T n, T m) { return m ? gcd(m, n % m) : n; }

template <typename X, typename T> constexpr T safe_mod(X x, const T &mod) {
  x %= mod;
  if (x < 0) x += mod;
  return x;
}

// ax+by = gcd(a,b) = g
// check: a>0 && b>0
// if a!=b: |x|<b && |y|<a
template <typename T> void exgcd(T a, T b, T &g, T &x, T &y) {
  if (!b) {
    g = a, x = 1, y = 0;
  } else {
    exgcd(b, a % b, g, y, x);
    y -= a / b * x;
  }
}

// m can be a composite number
template <typename T> T inverse(T a, T m) {
  T g, x, y;
  exgcd(a, m, g, x, y);
  if (g != 1) return -1; // no inverse element
  if (x < 0) x += m;
  return x;
}

struct barrett {
  unsigned int _m;
  unsigned long long im;

  explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}

  unsigned int umod() const { return _m; }

  unsigned int mul(unsigned int a, unsigned int b) const {
    unsigned long long z = a;
    z *= b;
    unsigned long long x = (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
    unsigned long long y = x * _m;
    return (unsigned int)(z - y + (z < y ? _m : 0));
  }
};

#endif