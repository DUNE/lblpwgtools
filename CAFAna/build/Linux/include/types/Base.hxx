#ifndef FHICLCPP_SIMPLE_TYPES_BASE_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_BASE_HXX_SEEN

#include <memory>
#include <string>

namespace fhicl {
class Base {
protected:
  static std::shared_ptr<Base> &empty() {
    static std::shared_ptr<Base> nullrtn(nullptr);
    nullrtn = nullptr;
    return nullrtn;
  }
  std::string internal_rep;
  Base() : internal_rep() {}
  Base(std::string const &str) : internal_rep(str) {}
  Base(std::string &&str) : internal_rep(std::move(str)) {}

  virtual ~Base(){};
  virtual void from(std::string const &) = 0;

public:
  virtual std::string to_string() const = 0;
  virtual std::string to_compact_string() const = 0;
  virtual std::string to_indented_string(size_t indent_level) const = 0;
  virtual std::string
  to_indented_string_with_src_info(size_t indent_level) const = 0;
};
} // namespace fhicl

#endif
