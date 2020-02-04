#ifndef FHICLCPP_SIMPLE_TYPES_ATOM_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_ATOM_HXX_SEEN

#include "types/Base.hxx"

#include "string_parsers/from_string.hxx"
#include "string_parsers/traits.hxx"

#include <iomanip>
#include <sstream>

namespace fhicl {
class Atom : public Base {
  void from(std::string const &str) { internal_rep = str; }
  void from(std::string &&str) { internal_rep = std::move(str); }

public:
  template <typename T>
  typename std::enable_if<
      !is_seq<T>::value && !std::is_same<T, std::string>::value, T>::type
  as() const {
    if (is_nil()) {
      return T{};
    }
    return string_parsers::str2T<T>(internal_rep);
  };
  template <typename T>
  typename std::enable_if<std::is_same<T, std::string>::value, T>::type
  as() const {
    if (is_nil()) {
      return "@nil";
    }
    std::string stringified = string_parsers::str2T<T>(internal_rep);
    size_t first_period = stringified.find_first_of(".");
    if (first_period !=
        std::string::npos) { // if you have found a period, test to see if it is
      // parseable to a double, if so, return without quotes.
      try {
        size_t pos;
        (void)stod(internal_rep, &pos);
        if(pos != internal_rep.size()){
          throw pos;
        }
        return stringified;
      } catch (...) {
      }
    }

    size_t first_punct = stringified.find_first_of(" ,\"\':;*&%$#@!~{}[]()/.");
    if (first_punct != std::string::npos) {
      std::stringstream ss("");
      ss << std::quoted(stringified);
      return ss.str();
    }
    return stringified;
  };
  Atom(std::string const &str) { from(str); }
  Atom(std::string &&str) { from(std::move(str)); }
  Atom(Atom const &other) : Base() { internal_rep = other.internal_rep; }
  Atom(Atom &&other) { internal_rep = std::move(other.internal_rep); }
  Atom() { internal_rep = "@nil"; }

  std::string to_string() const { return as<std::string>(); }
  std::string to_compact_string() const { return as<std::string>(); }
  std::string to_indented_string(size_t) const { return as<std::string>(); }
  std::string to_indented_string_with_src_info(size_t) const {
    return as<std::string>();
  }

  bool is_nil() const { return internal_rep == "@nil"; }
};
} // namespace fhicl

#endif
