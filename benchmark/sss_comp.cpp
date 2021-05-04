#include "sss_comp.hpp"

#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <tlx/cmdline_parser.hpp>
#include <tlx/die.hpp>
#include <unordered_map>

#include "sss.hpp"
#include "util/io.hpp"

class SSSBenchmark {
 public:
  std::string file_path;
  std::string algo = "ssss";

  SSSBenchmark() {}

  template <size_t tau>
  void run() {
    std::cout << "Running benchmark for"
              << " algo_name=" << algo
              << " tau=" << tau
              << " file=" << file_path
              << std::endl;

    // Prepare Data
    std::string text = sss::benchutil::read_file(file_path);

    if (algo == "ssss") {
      auto compressed_text = sss_comp<tau>(text);
      comp_info(text, compressed_text);
      comp_check<tau>(text, compressed_text);

      auto decompressed_text = sss_decomp(compressed_text);
      if(text != decompressed_text) {
        std::cout << "Decompressed Text wrong!" << '\n';
        std::cout << "Size: " << decompressed_text.size() << " should be " << text.size() << '\n';
        std::exit(EXIT_FAILURE);
      }

      /*
      auto ds = sss::string_syncronizing_set<tau>(text);
      auto sss = ds.get_sss();
      auto fps = ds.get_short_fps();

      die_unless(sss.size() == fps.size());
      std::cout << "Text size:        " << std::setw(12) << text.size() << '\n'
                << "SSS size :        " << std::setw(12) << sss.size() << '\n'
                << "Sampled every     " << std::setw(12) << (double)text.size() / sss.size() << '\n';

      std::unordered_map<uint64_t, std::string> map;

      size_t saved = 0;
      for (size_t i = 0; i < sss.size() - 1; ++i) {
        auto entry = map.find(fps[i]);
        if (entry == map.cend()) {
          map[fps[i]] = text.substr(sss[i], sss[i + 1] - sss[i]);
        } else {
          die_unless(strcmp(map[fps[i]].data(), text.data() + sss[i]));
          saved += map[fps[i]].size();
        }
      }

      size_t length = 0;
      for (auto& entry : map) {
        length += entry.second.size();
      }

      std::cout << "character saved : " << std::setw(12) << saved << '\n';
      std::cout << "beggining length: " << std::setw(12) << sss[0] << '\n';
      std::cout << "dict length     : " << std::setw(12) << length << '\n';
      std::cout << "dict entries    : " << std::setw(12) << map.size() << '\n';
      size_t approx_size = sss[0] + 8 * sss.size() + 8 * map.size() + length;
      std::cout << "approx size     : " << std::setw(12) << approx_size << '\n';
      std::cout << "approx compr    : " << std::setw(12) << static_cast<double>(approx_size) / text.size() << "\n\n";
      */
    }
  }
};

int32_t main(int32_t argc, char const* argv[]) {
  tlx::CmdlineParser cp;
  SSSBenchmark bench;
  cp.add_param_string("file", bench.file_path, "Path to the line-based input file.");
  cp.add_string('n', "name", bench.algo,
                "Name of the algorithm that is executed: "
                "[a] (default), [b], [c]");
  if (!cp.process(argc, argv)) {
    return EXIT_FAILURE;
  }

  bench.run<8>();
  bench.run<16>();
  bench.run<32>();
  bench.run<64>();
  bench.run<128>();
  bench.run<256>();
  bench.run<512>();

  return 0;
}
