#pragma once

#include <assert.h>

#include <cstdint>
#include <iostream>
#include <numeric>
#include <string_view>
#include <thread>
#include <vector>

#include "fingerprint.hpp"
#include "ring_buffer.hpp"
namespace sss {

template <size_t kTau = 1024, typename index_t = uint32_t>
class string_syncronizing_set_seq {
  static constexpr __int128 kPrime = 18446744073709551253ULL;
  static constexpr uint64_t TwoPowTauModQ = util::calculatePowerModulo(std::log2(kTau), kPrime);
  std::string_view m_text{};

 private:
  std::vector<index_t> m_sss;
  std::vector<uint64_t> m_fps;

 public:
  string_syncronizing_set_seq(std::string_view text) : m_text{text} {
    fill_synchronizing_set(text, 0, text.size() - (2 * kTau));
  }

  std::vector<index_t> get_sss() const {
    return m_sss;
  }

  std::vector<uint64_t> get_fps() const {
    return m_fps;
  }

  std::vector<uint64_t> get_short_fps() const {
    std::vector<uint64_t> fps{};
    for (size_t i = 0; i < m_sss.size() - 1; ++i) {
      unsigned __int128 fp{0};
      for (size_t j = m_sss[i]; j < m_sss[i + 1]; ++j) {
        fp *= 256;
        fp += static_cast<unsigned char>(m_text[j]);
        fp %= kPrime;
      }
      fps.push_back(fp);
    }
    unsigned __int128 fp{0};
    for (size_t j = m_sss.back(); j < m_text.size(); ++j) {
      fp *= 256;
      fp += static_cast<unsigned char>(m_text[j]);
      fp %= kPrime;
    }
    fps.push_back(fp);

    return fps;
  }

  size_t size() {
    return m_sss.size();
  }

 private:
  std::pair<std::vector<index_t>, std::vector<uint64_t>> fill_synchronizing_set(std::string_view text, const uint64_t from, const uint64_t to) {
    std::vector<index_t> sss;
    std::vector<uint64_t> fps;

    //Calculate first fingerprint
    unsigned __int128 fp = {0ULL};
    for (uint64_t i = 0; i < kTau; ++i) {
      fp *= 256;
      fp += (unsigned char)text[from + i];
      fp %= kPrime;
    }
    ring_buffer<uint64_t> fingerprints(4 * kTau);
    fingerprints.resize(from);
    fingerprints.push_back(static_cast<uint64_t>(fp));

    uint64_t min = 0;
    for (uint64_t i = from; i < to;) {
      // Compare this id with every other index which is not in q
      min = 0;
      size_t required = i + kTau - (fingerprints.size() - 1);
      calculate_fingerprints(text, required, fp, fingerprints);
      for (unsigned int j = 1; j <= kTau; ++j) {
        if (fingerprints[i + j] < fingerprints[i + min]) {
          min = j;
        }
      }
      if (min == 0 || min == kTau) {
        m_sss.push_back(i);
        m_fps.push_back(fingerprints[i]);
      }

      uint64_t local_min = i + min;
      ++i;
      calculate_fingerprints(text, 1, fp, fingerprints);
      while (i < to && i < local_min) {
        if (fingerprints[i + kTau] <= fingerprints[local_min]) {
          m_sss.push_back(i);
          m_fps.push_back(fingerprints[i]);
          if (fingerprints[i + kTau] != fingerprints[local_min]) {
            local_min = i + kTau;
          }
        }
        i++;
        calculate_fingerprints(text, 1, fp, fingerprints);
      }
    }
    return std::make_pair<>(sss, fps);
  }

  // Calculates the next count fingerprints
  void calculate_fingerprints(std::string_view text, size_t const count, unsigned __int128& fp,
                              ring_buffer<uint64_t>& fingerprints) const {
    for (uint64_t i = 0; i < count; ++i) {
      fp *= 256;
      fp += (unsigned char)text[kTau + fingerprints.size() - 1];
      fp %= kPrime;

      unsigned __int128 first_char_influence = text[fingerprints.size() - 1];
      first_char_influence *= TwoPowTauModQ;
      first_char_influence %= kPrime;

      if (first_char_influence < fp) {
        fp -= first_char_influence;
      } else {
        fp = kPrime - (first_char_influence - fp);
      }
      fingerprints.push_back(static_cast<uint64_t>(fp));
    }
  }
};
}  // namespace sss