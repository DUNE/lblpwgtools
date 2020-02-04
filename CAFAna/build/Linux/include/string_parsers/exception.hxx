#ifndef FHICLCPP_SIMPLE_STRING_PARSERS_EXCEPTION_HXX_SEEN
#define FHICLCPP_SIMPLE_STRING_PARSERS_EXCEPTION_HXX_SEEN

#include <sstream>
#include <stdexcept>

namespace fhicl {
namespace string_parsers {

struct fhicl_cpp_simple_except : public std::exception {
  std::stringstream msgstrm;
  std::string msg;
  fhicl_cpp_simple_except() : msgstrm(), msg() {}
  fhicl_cpp_simple_except(fhicl_cpp_simple_except const &other)
      : msgstrm(), msg() {
    msgstrm << other.msg;
    msg = other.msg;
  }
  const char *what() const noexcept { return msg.c_str(); }

  template <typename T> fhicl_cpp_simple_except &operator<<(T const &obj) {
    msgstrm << obj;
    msg = msgstrm.str();
    return (*this);
  }
};

#define NEW_EXCEPT(EXCEPT_NAME)                                                \
  struct EXCEPT_NAME : public fhicl_cpp_simple_except {                        \
    EXCEPT_NAME() : fhicl_cpp_simple_except() {}                               \
    EXCEPT_NAME(EXCEPT_NAME const &other) : fhicl_cpp_simple_except(other) {}  \
    template <typename T> EXCEPT_NAME &operator<<(T const &obj) {              \
      msgstrm << obj;                                                          \
      msg = msgstrm.str();                                                     \
      return (*this);                                                          \
    }                                                                          \
  }

NEW_EXCEPT(mismatched_brackets);
NEW_EXCEPT(parser_fail);
NEW_EXCEPT(wrong_number_of_elements);

#undef NEW_EXCEPT

} // namespace string_parsers
} // namespace fhicl

#endif
