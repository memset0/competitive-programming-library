#ifndef MEM_BIT_H
#define MEM_BIT_H

#if __cplusplus >= 202002L
using std::bit_ceil;
#else
inline unsigned int bit_ceil(unsigned int n) {
  unsigned int x = 1;
  while (x < (unsigned int)(n)) x *= 2;
  return x;
}
#endif

inline int countr_zero(unsigned int n) {
#ifdef _MSC_VER
  unsigned long index;
  _BitScanForward(&index, n);
  return index;
#else
  return __builtin_ctz(n);
#endif
}

#endif