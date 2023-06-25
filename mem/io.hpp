#ifndef MEM_IO_H
#define MEM_IO_H

#include "global.hpp"
#include <cmath>
#include <cstdio>
#include <string>

// forked from mcfx's code

#define RX                                        \
  x = 0;                                          \
  char t = P();                                   \
  while ((t < 48 || t > 57) && t != '-') t = P(); \
  bool f = 0;                                     \
  if (t == '-') t = P(), f = 1;                   \
  x = t - 48;                                     \
  for (t = P(); t >= 48 && t <= 57; t = P()) x = x * 10 + t - 48

#define RL                                                                  \
  if (t == '.') {                                                           \
    lf u = .1;                                                              \
    for (t = P(); t >= 48 && t <= 57; t = P(), u *= 0.1) x += u * (t - 48); \
  }                                                                         \
  if (f) x = -x

#define RU                          \
  x = 0;                            \
  char t = P();                     \
  while (t < 48 || t > 57) t = P(); \
  x = t - 48;                       \
  for (t = P(); t >= 48 && t <= 57; t = P()) x = x * 10 + t - 48

#define WI(S, T)                             \
  if (x) {                                   \
    if (x < 0) {                             \
      P('-'), x = -x;                        \
      if (x < 0) {                           \
        *this, (T)x;                         \
        return *this;                        \
      }                                      \
    }                                        \
    unsigned char s[S], c = 0;               \
    while (x) s[c++] = x % 10 + 48, x /= 10; \
    while (c--) P(s[c]);                     \
  } else                                     \
    P(48)

#define WL                          \
  if (y) {                          \
    lf t = 0.5;                     \
    for (int i = y; i--;) t *= 0.1; \
    if (x >= 0) x += t;             \
    else                            \
      x -= t, P('-');               \
    *this, (ll)(abs(x));            \
    P('.');                         \
    if (x < 0) x = -x;              \
    while (y--) {                   \
      x *= 10;                      \
      x -= floor(x * 0.1) * 10;     \
      P(((int)x) % 10 + 48);        \
    }                               \
  } else if (x >= 0)                \
    *this, (ll)(x + 0.5);           \
  else                              \
    *this, (ll)(x - 0.5);

#define WU(S)                                \
  if (x) {                                   \
    char s[S], c = 0;                        \
    while (x) s[c++] = x % 10 + 48, x /= 10; \
    while (c--) P(s[c]);                     \
  } else                                     \
    P(48)

constexpr bool IS(char x) { return x == 10 || x == 13 || x == ' '; }

template <typename T> struct FastRead {
  T P;
  inline FastRead &operator,(int &x) {
    RX;
    if (f) x = -x;
    return *this;
  }
  inline operator int() {
    int x;
    *this, x;
    return x;
  }
  inline FastRead &operator,(ll &x) {
    RX;
    if (f) x = -x;
    return *this;
  }
  inline operator ll() {
    ll x;
    *this, x;
    return x;
  }
  inline FastRead &operator,(char &x) {
    for (x = P(); IS(x); x = P())
      ;
    return *this;
  }
  inline operator char() {
    char x;
    *this, x;
    return x;
  }
  inline FastRead &operator,(char *x) {
    char t = P();
    for (; IS(t) && ~t; t = P())
      ;
    if (~t) {
      for (; !IS(t); t = P()) *x++ = t;
    }
    *x++ = 0;
    return *this;
  }
  inline FastRead &operator,(std::string &x) {
    char t = P();
    for (; IS(t) && ~t; t = P())
      ;
    if (~t) {
      for (; !IS(t); t = P()) x += t;
    }
    return *this;
  }
  inline FastRead &operator,(lf &x) {
    RX;
    RL;
    return *this;
  }
  inline operator lf() {
    lf x;
    *this, x;
    return x;
  }
  inline FastRead &operator,(llf &x) {
    RX;
    RL;
    return *this;
  }
  inline operator llf() {
    llf x;
    *this, x;
    return x;
  }
  inline FastRead &operator,(unsigned &x) {
    RU;
    return *this;
  }
  inline operator unsigned() {
    unsigned x;
    *this, x;
    return x;
  }
  inline FastRead &operator,(ull &x) {
    RU;
    return *this;
  }
  inline operator ull() {
    ull x;
    *this, x;
    return x;
  }
  void file(const char *x) { P.file(x); }
};
struct Fwp {
  int p;
};
Fwp prec(int x) { return (Fwp){x}; }
template <typename T> struct FastWrite {
  T P;
  int p;
  inline FastWrite &operator,(int x) {
    WI(10, unsigned);
    return *this;
  }
  inline FastWrite &operator,(unsigned x) {
    WU(10);
    return *this;
  }
  inline FastWrite &operator,(ll x) {
    WI(19, ull);
    return *this;
  }
  inline FastWrite &operator,(ull x) {
    WU(20);
    return *this;
  }
  inline FastWrite &operator,(char x) {
    P(x);
    return *this;
  }
  inline FastWrite &operator,(const char *x) {
    while (*x) P(*x++);
    return *this;
  }
  inline FastWrite &operator,(const Fwp &x) {
    p = x.p;
    return *this;
  }
  inline FastWrite &operator,(lf x) {
    int y = p;
    WL;
    return *this;
  }
  inline FastWrite &operator()(lf x, int y) {
    WL;
    return *this;
  }
  inline FastWrite &operator,(llf x) {
    int y = p;
    WL;
    return *this;
  }
  inline FastWrite &operator()(llf x, int y) {
    WL;
    return *this;
  }
  void file(const char *x) { P.file(x); }
  void flush() { P.flush(); }
};
#ifdef LOCAL
struct Cg {
  inline char operator()() { return getchar(); }
  void file(const char *f) { freopen(f, "r", stdin); }
};
struct Cp {
  inline void operator()(char x) { putchar(x); }
  void file(const char *f) { freopen(f, "w", stdout); }
  void flush() { fflush(stdout); }
};
struct Cpr {
  inline void operator()(char x) { fputc(x, stderr); }
  void file(const char *f) { freopen(f, "w", stderr); }
  void flush() { fflush(stderr); }
};
template <typename T> struct Fd {
  FastWrite<T> *o;
  template <typename P> inline Fd &operator,(P x) {
    (*o), x, ' ';
    return *this;
    ;
  }
  ~Fd() { (*o), '\n'; }
};
template <typename T> struct Fds {
  FastWrite<T> *o;
  template <typename P> inline Fd<T> operator,(P x) {
    (*o), x, ' ';
    return (Fd<T>){o};
  }
};
FastWrite<Cpr> err;
Fds<Cpr> dbg{&err};
#else
#define BSZ 131072
struct Cg {
  char t[BSZ + 1], *o, *e;
  Cg() { e = o = t + BSZ; }
  inline char operator()() {
    if (o == e) t[fread(o = t, 1, BSZ, stdin)] = 0;
    return *o++;
  }
  void file(const char *f) { freopen(f, "r", stdin); }
};
struct Cp {
  char t[BSZ + 1], *o, *e;
  Cp() { e = (o = t) + BSZ; }
  inline void operator()(char p) {
    if (o == e) fwrite(o = t, 1, BSZ, stdout);
    *o++ = p;
  }
  void file(const char *f) { freopen(f, "w", stdout); }
  void flush() { fwrite(t, 1, o - t, stdout), o = t, fflush(stdout); }
  ~Cp() { fwrite(t, 1, o - t, stdout); }
};
#endif

FastRead<Cg> in;
FastWrite<Cp> out;
const char dl = '\n';

#endif