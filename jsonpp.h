#ifndef _JSONPP_H
#define _JSONPP_H

#include <memory>
#include <string>        // for key names.
#include <unordered_map> // for storing json values.
#include <variant>

namespace jsonpp {

enum json_type_t {
  T_NONE,
  T_ARRAY,
  T_NUMBER,
  T_OBJECT,
  T_STRING,
};

class json_value_t;
struct object_t {
  object_t();
  json_value_t &operator[](const std::string &val) { return values_[val]; }
  std::unordered_map<std::string, json_value_t> values_;
};

class json_value_t {
private:
  json_type_t type_{T_NONE};
  std::variant<object_t, std::string, int64_t,  std::monostate> value_{
      std::monostate{}};
};

class json_t {
public:
  json_t();

private:
  object_t root_obj_;
};

} // namespace jsonpp

#endif
