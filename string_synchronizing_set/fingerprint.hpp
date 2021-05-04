#pragma once

#include <cmath>
namespace sss::util {

static constexpr uint64_t calculatePowerModulo(unsigned int const power,
                                               __int128 const kPrime) {
  unsigned __int128 x = 256;
  for (unsigned int i = 0; i < power; i++) {
    x = (x * x) % kPrime;
  }
  return static_cast<uint64_t>(x);
}

static constexpr size_t kTau = 1024;
static constexpr __int128 kPrime = 18446744073709551253ULL;
static constexpr uint64_t TwoPowTauModQ = calculatePowerModulo(std::log2(kTau), kPrime);
}  // namespace sss::util