#ifndef _JSONPP_H
#define _JSONPP_H

#include <cctype>
#include <cstddef>
#include <memory>
#include <string>        // for key names.
#include <unordered_map> // for storing json values.
#include <variant>
#include <vector>

namespace jsonpp {

enum json_type_t {
  J_NONE,
  J_ARRAY,
  J_NUMBER,
  J_OBJECT,
  J_STRING,
};

enum token_type_t {
  T_NONE,
  T_LEFT_BRACE,
  T_LEFT_BRACKET,
  T_RIGHT_BRACE,
  T_RIGHT_BRACKET,
  T_COLON,
  T_COMMA,
  T_NUMBER,
  T_STRING,
  T_EOF,
};

class json_value_t;
struct object_t {
  object_t();
  json_value_t &operator[](const std::string &val) { return values_[val]; }
  std::unordered_map<std::string, json_value_t> values_;
};

struct json_arr_t {
  std::vector<json_value_t> values;
};

struct token_t {
  template <typename T>
  token_t(token_type_t ty, T data) : type_(ty), data_(data) {}
  token_t(token_type_t ty) : type_(ty){};

  token_type_t type_{T_NONE};
  std::variant<std::string, int64_t, std::monostate> data_{std::monostate{}};
};

class json_value_t {
private:
  json_type_t type_{J_NONE};
  std::variant<object_t, std::string, int64_t, json_arr_t, std::monostate>
      value_{std::monostate{}};
};

class json_t {
public:
  json_t();

private:
  std::vector<token_t> parse_tokens(const std::string &p) {
    std::vector<token_t> res;
    size_t index = 0;
    auto sz = p.size();

    while (index < sz) {
      if (std::isspace(p[index])) {
        ++index;
        continue;
      }

      if (std::isdigit(p[index])) {
        std::string num_str = std::string(1, p[index]);
        ++index;

        while (index < sz && std::isdigit(p[index])) {
          num_str += p[index];
          ++index;
        }
        res.push_back(token_t(T_NUMBER, num_str));
        continue;
      }

      if (p[index] == '"') {
        std::string str;
        ++index;

        while (index < sz && p[index] != '"') {
          str += p[index];
          ++index;
        }
        ++index;

        res.push_back(token_t(T_STRING, str));
        continue;
      }

      switch (p[index]) {
      case '{': {
        ++index;
        res.push_back(token_t(T_LEFT_BRACE));
        continue;
      }
      case '[': {
        ++index;
        res.push_back(token_t(T_LEFT_BRACKET));
        continue;
      }
      case ']': {
        ++index;
        res.push_back(token_t(T_RIGHT_BRACKET));
        continue;
      }
      case '}': {
        ++index;
        res.push_back(token_t(T_RIGHT_BRACKET));
        continue;
      }
      case ',': {
        ++index;
        res.push_back(token_t(T_COMMA));
        continue;
      }
      case ':': {
        ++index;
        res.push_back(token_t(T_COLON));
        continue;
      }
      default:
        break; // unrecognized token
      }
    }
    res.push_back(token_t(T_EOF));
    return res;
  }

  object_t root_obj_;
};

} // namespace jsonpp

#endif
