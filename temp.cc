#include "jsonpp.h"
#include <fstream>
#include <iostream>
#include <type_traits>

int main() {
  jsonpp::json_t val;
  std::ifstream t("test.json");
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);

  val.parse(buffer);
  const auto &obj = std::get<jsonpp::object_t>(val.root_.value_);
  std::cout << obj.values_.size() << '\n';
  std::cout << obj.values_.at("hello").type_  << '\n';

  std::cout << "root value type: " << val.root_.type_ << '\n';

  return 0;
}
