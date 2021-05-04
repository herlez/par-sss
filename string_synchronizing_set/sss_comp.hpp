#include <cmath>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "sss.hpp"
#include "sss_seq.hpp"
struct sss_comp_type {
  std::string begin{};
  std::string end{};
  std::vector<uint64_t> ind;
  std::unordered_map<uint64_t, std::string> dict;
};

template <size_t tau>
sss_comp_type sss_comp(std::string_view text) {
  size_t size = 0;

  sss_comp_type compressed_text;
  auto &ind = compressed_text.ind;
  auto &dict = compressed_text.dict;

  auto const ds = sss::string_syncronizing_set_seq<tau>(text);
  auto const sss = ds.get_sss();
  auto const fps = ds.get_short_fps();
  compressed_text.begin = text.substr(0, sss[0]);
  compressed_text.end = text.substr(sss.back());
  for (size_t i = 0; i < sss.size() - 1; ++i) {
    if (!dict.contains(fps[i])) {
      dict[fps[i]] = text.substr(sss[i], sss[i + 1] - sss[i]);
    }
    if (dict[fps[i]].size() != sss[i + 1] - sss[i]) {  //CHeck
      //std::cout << "WTF at " << sss[i] << '\n';
    }
    ind.push_back(fps[i]);
    size += dict[fps[i]].size();
  }
  return compressed_text;
}

std::string sss_decomp(sss_comp_type &comp) {
  std::string decomp{};
  decomp.append(comp.begin);
  for (auto fp : comp.ind) {
    decomp.append(comp.dict[fp]);
  }
  decomp.append(comp.end);

  return decomp;
}

void comp_info(std::string_view text, sss_comp_type &comp) {
  size_t size = 0;
  for (auto &e : comp.dict) {
    size += e.second.size();
  }

  double log_bit = std::ceil(log2(comp.dict.size()));
  size_t approx_size = comp.begin.size() + (log_bit * comp.ind.size())/8 + (log_bit * comp.dict.size())/8 + size + comp.end.size();
  std::cout << "Text size:    " << std::setw(12) << text.size() << '\n'
            << "Begin:        " << std::setw(12) << comp.begin.size() << '\n'
            << "End:          " << std::setw(12) << comp.end.size() << '\n'
            << "#Indexes:     " << std::setw(12) << comp.ind.size() << '\n'
            << "#Map entries: " << std::setw(12) << comp.dict.size() << '\n'
            << "Dict size:    " << std::setw(12) << size << '\n'
            << "Log:          " << std::setw(12) << log_bit << '\n'
            << "approx compr: " << std::setw(12) << static_cast<double>(approx_size) / text.size() << "\n\n";

}

template <size_t tau>
void comp_check(std::string_view text, sss_comp_type &comp) {
  auto ds = sss::string_syncronizing_set_seq<tau>(text);
  auto sss = ds.get_sss();
  size_t mismatched_factors = 0;

  if (0 != std::memcmp(comp.begin.data(), text.begin(), sss[0])) {
    std::cout << "Start does not match";
  }
  for (size_t i = 0; i < sss.size() - 1; ++i) {
    if (0 != std::memcmp(comp.dict[comp.ind[i]].data(), text.begin() + sss[i], sss[i + 1] - sss[i])) {
      std::cout << "Factor " << sss[i] << '-' << sss[i + 1] << " with FP: " << std::hex << comp.ind[i] << std::dec << '\n'
                << comp.dict[comp.ind[i]] << " but should be:\n"
                << text.substr(sss[i], sss[i + 1] - sss[i]) << '\n';
      ++mismatched_factors;
    }
  }
  if (0 != std::memcmp(comp.end.data(), text.begin() + sss.back(), text.size() - sss.back())) {
    std::cout << "End does not match";
  }
  std::cout << "Mismatched factors: " << mismatched_factors << '\n';
}
