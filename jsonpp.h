#ifndef _JSONPP_H
#define _JSONPP_H

#include <cctype>
#include <cstddef>
#include <memory>
#include <optional>
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

enum status_t {
  S_OK,
  S_EXPECTED_OTHER,
  S_FAIL,
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
  template <typename T>
  json_value_t(json_type_t ty, T data) : type_(ty), value_(data) {}

  template <typename T>
  json_value_t(json_type_t ty, T &&data) : type_(ty), value_(std::move(data)) {}

private:
  json_type_t type_{J_NONE};
  std::variant<object_t, std::string, int64_t, json_arr_t, std::monostate>
      value_{std::monostate{}};
};

class json_t {
public:
  json_t();

  status_t parse(const std::string &p) {
    auto tokens = parse_tokens(p);
    size_t pos = 0;

    while (tokens[pos].type_ != T_EOF) {
      // in this loop objects can either be arrays or
      // they can be objects.
      if (tokens[pos].type_ == T_LEFT_BRACE) {

      } else if (tokens[pos].type_ == T_LEFT_BRACKET) {
      } else {
        return S_FAIL; // unrecognized token at this point.
      }
    }

    return S_OK;
  }

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

  bool match(const std::vector<token_t> &tokens, size_t &pos, token_type_t ty) {
    return tokens[pos].type_ == ty;
  }

  std::optional<object_t> parse_obj(const std::vector<token_t> &tokens,
                                    size_t &pos) noexcept {
    object_t res;
    // objects have a string key, and then followed by a colon. After that we
    // have either an object, an array, a number or a string.
    ++pos; // skip the brace.

    if (!match(tokens, pos, T_STRING)) {
      return std::nullopt;
    }
    auto key = std::get<std::string>(tokens[pos].data_);
    ++pos;
    if (!match(tokens, pos, T_COLON)) {
      return std::nullopt;
    }
    ++pos;

    auto value = parse_json_value(tokens, pos);
    if (!value.has_value()) {
      return std::nullopt;
    }
    res[key] = std::move(value.value());

    return res;
  }

  std::optional<json_value_t>
  parse_json_value(const std::vector<token_t> &tokens, size_t &pos) noexcept {
    if (tokens[pos].type_ == T_NUMBER) {
      json_value_t val;
    }

    return std::nullopt;
  }

  json_value_t root_{};
};

} // namespace jsonpp

#endif
