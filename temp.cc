#include "jsonpp.h"
#include <fstream>
#include <iostream>

int main() {
  jsonpp::json_t val;
  std::ifstream t("test.json");
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);

  val.parse(buffer);

  std::cout << "root value type: " << val.root_.type_ << '\n';

  return 0;
}
