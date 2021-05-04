#include "sss.hpp"

#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <tlx/die.hpp>
#include <unordered_map>

#include "../benchmark/util/io.hpp"

int main(int argc, char const* argv[]) {
  //std::string text = sss::benchutil::read_file("../../data/the_three_brothers.txt");
  std::string text = sss::benchutil::read_file("../../data/dna");

  auto const ds = sss::string_syncronizing_set<64>(text);
  auto const sss = ds.get_sss();

  {
    auto const fps = ds.get_fps();
    die_unless(sss.size() == fps.size());
    std::cout << "Text size: " << text.size() << '\n'
              << "SSS size : " << sss.size() << '\n';

    std::unordered_map<uint64_t, std::string> dict;
    for (size_t i = 0; i < sss.size() - 1; ++i) {
      auto entry = dict.find(fps[i]);
      if (entry == dict.cend()) {
        dict[fps[i]] = text.substr(sss[i], sss[i + 1] - sss[i]);
      } else {
        die_unless(strcmp(dict[fps[i]].data(), text.data() + sss[i]));
      }
    }
  }
  
  {
    auto const fps = ds.get_short_fps();
    die_unless(sss.size() == fps.size());
    std::cout << "Text size: " << text.size() << '\n'
              << "SSS size : " << sss.size() << '\n';

    std::unordered_map<uint64_t, std::string> dict;
    for (size_t i = 0; i < sss.size() - 1; ++i) {
      auto entry = dict.find(fps[i]);
      if (entry == dict.cend()) {
        dict[fps[i]] = text.substr(sss[i], sss[i + 1] - sss[i]);
      } else {
        die_unless(strcmp(dict[fps[i]].data(), text.data() + sss[i]));
      }
    }
  }
  return 0;
}
