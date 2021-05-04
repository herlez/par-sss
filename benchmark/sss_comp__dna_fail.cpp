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
      auto const ds = sss::string_syncronizing_set<tau>(text);
      auto const sss = ds.get_sss();
      std::unordered_map<size_t, size_t> sss_inverse{};
      for(size_t i = 0; i < sss.size(); ++i) {
        sss_inverse[sss[i]] = i; 
      } 

      std::cout << "Text size:        " << std::setw(12) << text.size() << '\n'
                << "First pos:        " << std::setw(12) << sss[0] << '\n'
                << "SSS size :        " << std::setw(12) << sss.size() << '\n'
                << "Sampled every     " << std::setw(12) << (double)text.size() / sss.size() << "\n\n";

      std::vector<std::vector<uint64_t>> fps{2};
      fps[0] = ds.get_fps();
      fps[1] = ds.get_short_fps();

      std::vector<std::unordered_map<uint64_t, std::string>> dict{2};
      std::vector<std::unordered_map<uint64_t, std::vector<size_t>>> dict_pos{2};

      for (size_t i = 0; i < sss.size() - 1; ++i) {
        uint64_t fp0 = fps[0][i];
        uint64_t fp1 = fps[1][i];

        dict_pos[0][fp0].push_back(sss[i]);
        dict_pos[1][fp1].push_back(sss[i]);

        if (dict_pos[0][fp0].size() > dict_pos[1][fp1].size()) {
          for (auto& pos : dict_pos[0][fp0]) {
            std::cout << pos << " from " << sss[sss_inverse[pos]] << " to " << sss[sss_inverse[pos]+1] << '\n';
            std::cout << text.substr(pos, sss[sss_inverse[pos]+1] - sss[sss_inverse[pos]]) << " " << 
            text.substr(pos + sss[sss_inverse[pos]+1] - sss[sss_inverse[pos]], 32) << '\n';
            std::cout << "FP0: " << fps[0][sss_inverse[pos]] << '\n';
            std::cout << "FP1: " << fps[1][sss_inverse[pos]] << "\n\n";
          }
          std::cout << '\n';
          for (auto& pos : dict_pos[1][fp0]) {
            std::cout << pos << '\n';
            std::cout << pos << " from " << sss[sss_inverse[pos]] << " to " << sss[sss_inverse[pos]+1] << '\n';
            std::cout << text.substr(pos, sss[sss_inverse[pos]+1] - sss[sss_inverse[pos]]) << " " << 
            text.substr(pos + sss[sss_inverse[pos]+1] - sss[sss_inverse[pos]], 32) << '\n';
            std::cout << "FP0: " << fps[0][sss_inverse[pos]] << '\n';
            std::cout << "FP1: " << fps[1][sss_inverse[pos]] << "\n\n";
          }
          std::cout << '\n';
          std::exit(EXIT_FAILURE);
        }
      }
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

  bench.run<32>();

  return 0;
}
