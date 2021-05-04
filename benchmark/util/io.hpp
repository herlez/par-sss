#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace sss::benchutil {

std::vector<std::string> read_strings_line_by_line(std::string const& path) {
  std::fstream stream(path.c_str(), std::ios::in);
  std::string cur_line;

  std::vector<std::string> result;

  if (stream) {
    while (std::getline(stream, cur_line)) {
      result.push_back(cur_line);
    }
  }
  stream.close();

  return result;
}

std::string read_file(std::string const& path) {
  std::ifstream stream(path.c_str());

  return std::string((std::istreambuf_iterator<char>(stream)),
                     (std::istreambuf_iterator<char>()));
}
}  // namespace sss::util