#ifndef FHICLCPP_SIMPLE_TYPES_EXCEPTION_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_EXCEPTION_HXX_SEEN

#include <sstream>
#include <stdexcept>

namespace fhicl {

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

NEW_EXCEPT(null_key);
NEW_EXCEPT(parser_fail);
NEW_EXCEPT(invalid_key);
NEW_EXCEPT(nonexistant_key);
NEW_EXCEPT(cant_insert);
NEW_EXCEPT(wrong_fhicl_category);
NEW_EXCEPT(bizare_error);

#undef NEW_EXCEPT

} // namespace fhicl

#endif
