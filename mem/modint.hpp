#ifndef MEM_MODINT_H
#define MEM_MODINT_H

#include "io.hpp"
#include "math.hpp"

template <const int mod> struct Modulus {
  int x;
  Modulus() {}
  explicit Modulus(int t) { x = t; }
  explicit Modulus(long long t) { x = safe_mod(t, mod); }

  constexpr void operator*=(Modulus a) { x = (long long)x * a.x % mod; }
  constexpr void operator-=(Modulus a) { (x -= a.x) < 0 && (x += mod); }
  constexpr void operator+=(Modulus a) { (x += a.x) >= mod && (x -= mod); }

  friend constexpr Modulus operator*(const Modulus &a, const Modulus &b) { return (long long)a.x * b.x % mod; }
  friend constexpr Modulus operator-(Modulus a, const Modulus &b) { return ((a.x -= b.x) < 0 && (a.x += mod)), a; }
  friend constexpr Modulus operator+(Modulus a, const Modulus &b) { return ((a.x += b.x) >= mod && (a.x -= mod)), a; }
};

template <typename T, const int mod> inline FastRead<T> &operator,(const FastRead<T> &r, Modulus<mod> &x) { return r, x.x; }
template <typename T, const int mod> inline FastWrite<T> &operator,(const FastWrite<T> &w, const Modulus<mod> &x) { return w, x.x; }

template <const int mod> inline Modulus<mod> finv(Modulus<mod> x) {
  if (x.x < 2) return x;
  return (mod - mod / x.x) * finv(mod % x.x);
}

template <const int mod, typename T> inline Modulus<mod> fpow(Modulus<mod> a, T b) {
  Modulus<mod> s = 1;
  for (; b; b >>= 1, a = a * a)
    if (b & 1) s = s * a;
  return s;
}

template <const int mod> void init_inverse(int n, Modulus<mod> *inv) {
  inv[0] = inv[1] = 1;
  for (int i = 2; i < n; i++) inv[i] = (mod - mod / i) * inv[mod % i];
}
template <const int mod> void init_factorial(int n, Modulus<mod> *fac, Modulus<mod> *ifac) {
  fac[0] = 1, init_inverse(n, ifac);
  for (int i = 1; i < n; i++) fac[i] = fac[i - 1] * i, ifac[i] = ifac[i - 1] * ifac[i];
}

#define Modulus ModInt

#endif