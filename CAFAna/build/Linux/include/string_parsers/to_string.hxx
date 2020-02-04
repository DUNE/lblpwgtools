#ifndef FHICLCPP_SIMPLE_STRING_PARSERS_TO_STRING_HXX_SEEN
#define FHICLCPP_SIMPLE_STRING_PARSERS_TO_STRING_HXX_SEEN

#include "string_parsers/traits.hxx"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace fhicl {
namespace string_parsers {

template <typename T>
inline std::string
T2Str(typename std::enable_if<!is_seq<T>::value, T>::type const &o) {
  std::stringstream ss("");
  ss << o;
  return ss.str();
}

template <> inline std::string T2Str<bool>(bool const &o) {
  return o ? "true" : "false";
}
template <> inline std::string T2Str<std::string>(std::string const &o) {
  std::stringstream ss("");
  ss << std::quoted(o);
  return ss.str();
}

template <typename T>
inline std::string
T2Str(typename std::enable_if<is_pair<T>::value, T>::type const &pair);
template <typename T>
inline std::string
T2Str(typename std::enable_if<is_tuple<T>::value, T>::type const &t);

template <typename T>
inline std::string
T2Str(typename std::enable_if<is_vect<T>::value || is_array<T>::value,
                              T>::type const &arr_like) {
  std::stringstream ss("");
  ss << "[";
  size_t N = arr_like.size();
  for (size_t i = 0; i < N; ++i) {
    ss << T2Str<typename T::value_type>(arr_like[i])
       << (i + 1 == arr_like.size() ? "" : ",");
  }
  ss << "]";
  return ss.str();
}

template <typename T>
inline std::string
T2Str(typename std::enable_if<is_pair<T>::value, T>::type const &pair) {
  std::stringstream ss("");
  ss << "[" << T2Str<typename T::first_type>(pair.first) << ","
     << T2Str<typename T::second_type>(pair.second) << "]";
  return ss.str();
}

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), std::ostream &>::type
write_tuple(std::ostream &os, std::tuple<Tp...> const &) {
  return os << "]";
}

template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if <
    I<sizeof...(Tp), std::ostream &>::type
    write_tuple(std::ostream &os, std::tuple<Tp...> const &t) {
  if (I == 0) {
    os << "[";
  }
  os << T2Str<typename std::tuple_element<I, std::tuple<Tp...>>::type>(
      std::get<I>(t));
  if ((I + 1) != std::tuple_size<std::tuple<Tp...>>::value) {
    os << ",";
  }
  return write_tuple<I + 1, Tp...>(os, t);
}

template <typename T>
inline std::string
T2Str(typename std::enable_if<is_tuple<T>::value, T>::type const &t) {
  std::stringstream ss("");
  write_tuple(ss, t);
  return ss.str();
}

} // namespace string_parsers
} // namespace fhicl

#endif
