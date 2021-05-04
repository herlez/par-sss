#include <iostream>
#include <random>
#include <tlx/cmdline_parser.hpp>
#include <tlx/math/aggregate.hpp>

#include "sss.hpp"
#include "sss_seq.hpp"
#include "util/io.hpp"
#include "util/spacer.hpp"
#include "util/timer.hpp"

class SSSBenchmark {
 public:
  std::string file_path;
  uint64_t runs = 5;
  std::string algo = "sss_par";
  size_t num_threads = 1;

  SSSBenchmark() {}

  void run() {
    std::cout << "Running benchmark for"
              << " runs=" << runs
              << " file=" << file_path
              << std::endl;

    // Prepare Data
    std::string text = sss::benchutil::read_file(file_path);

    // Run Algorithm
    tlx::Aggregate<size_t> construction_times;
    tlx::Aggregate<size_t> mem_construction;
    tlx::Aggregate<size_t> sss_size;
    for (uint64_t i = 0; i < runs; ++i) {
      if (num_threads == 1) {
        sss::benchutil::spacer spacer;
        sss::benchutil::timer timer;
        auto sss = sss::string_syncronizing_set(text, num_threads);

        construction_times.add(timer.get());
        mem_construction.add(spacer.get_peak());
        sss_size.add(sss.size());
      } else {
        sss::benchutil::spacer spacer;
        sss::benchutil::timer timer;
        auto sss = sss::string_syncronizing_set_seq(text);

        construction_times.add(timer.get());
        mem_construction.add(spacer.get_peak());
        sss_size.add(sss.size());
      }
    }

    std::cout << "RESULT algo=" << "ssss"
              << " threads=" << num_threads
              << " size=" << text.size()
              << " runs=" << runs
              << " construction_time_avg=" << construction_times.avg()
              << " sss_size=" << sss_size.avg()
              << " memory_construction_avg=" << static_cast<size_t>(mem_construction.avg()) << '\n';
  }

 public:
};  // class RMQBenchmark

int32_t main(int32_t argc, char const* argv[]) {
  tlx::CmdlineParser cp;
  SSSBenchmark bench;
  cp.add_param_string("file", bench.file_path, "Path to the line-based input file.");
  cp.add_bytes('r', "runs", bench.runs,
               "Number of runs the benchmark is executed");
  cp.add_bytes('t', "threads", bench.num_threads,
               "Number of threads");
  if (!cp.process(argc, argv)) {
    return 0;
  };

  bench.run();

  return 0;
}