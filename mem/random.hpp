#include <chrono>
#include <random>

const int LuckyNumber = 0726; // It's Kanbe Kotori's birthday!
std::mt19937 rng(LuckyNumber ^ std::chrono::steady_clock::now().time_since_epoch().count());
std::mt19937_64 rng64(LuckyNumber ^ std::chrono::steady_clock::now().time_since_epoch().count());

template <typename T> inline T rand(T l, T r) { return std::uniform_int_distribution<T>(l, r)(rng); }
template <typename T> inline T rand64(T l, T r) { return std::uniform_int_distribution<T>(l, r)(rng64); }