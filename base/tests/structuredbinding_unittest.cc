#include <iostream>
#include <map>

int main(int argc, char **argv) {
  std::map<std::string, int> cities;

  cities["guangzhou"] = 0;
  cities["zhengzhou"] = 1;
  cities["beijing"]   = 2;
  cities["shanghai"]  = 3;

  for (const auto &[cityname, citynumber] : cities) {
    std::cout << cityname << " : " << citynumber << std::endl;
  }

  return 0;
}
