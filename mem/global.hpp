#ifndef MEM_GLOBAL_H
#define MEM_GLOBAL_H

#include <iostream>

// optimize
#define OP operator
#define TT template <class T>
#define IL __attribute__((always_inline)) inline

// short stl
#define xx first
#define yy second
#define pb push_back
#define eb emplace_back
#define mp(a, b) std::make_pair(a, b)

// type
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef double lf;
typedef long double llf;

#ifdef __SIZEOF_INT128__
typedef __int128_t lll;
typedef __uint128_t ulll;
#endif

typedef std::pair<int, int> pii;

// utils
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

// fast coding
#define rep(i, l, r) for (int i = (l), i##end = (r); i <= i##end; i++)
#define per(i, l, r) for (int i = (l), i##end = (r); i >= i##end; i--)

#endif