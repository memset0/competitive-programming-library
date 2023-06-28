#include "mem/global.hpp"
#include "mem/io.hpp"
#include "mem/math.hpp"
#include "mem/modint.hpp"
#include "mem/random.hpp"
#include <bits/stdc++.h>

const int mod = 998244353;

int main() {
  in.file("test.in");
  // out.file("test.out");

  int T;
  in, T;
  while (T--) {
    std::string name;
    in, name;
    out, "[", name.c_str(), "]", dl;
    if (name == "basic_io") {
      int a;
      long long b;
      double c;
      long double d;
      in, a, b, c, d;
      out, a, '\n', b, '\n';
      out(c, 3), out, '\n';
      out(d, 3), out, '\n';
    } else {
      out, "skipped.";
    }
  }
}
