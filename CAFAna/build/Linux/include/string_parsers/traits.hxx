#ifndef FHICLCPP_SIMPLE_STRING_PARSERS_TRAITS_HXX_SEEN
#define FHICLCPP_SIMPLE_STRING_PARSERS_TRAITS_HXX_SEEN

#include <array>
#include <iostream>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

namespace fhicl {
template <typename T> struct is_vect { static constexpr bool value = false; };
template <typename... Ts> struct is_vect<std::vector<Ts...>> {
  static constexpr bool value = true;
};
template <typename T> struct is_array { static constexpr bool value = false; };
template <typename T, size_t N> struct is_array<std::array<T, N>> {
  static constexpr bool value = true;
};
template <typename T> struct is_tuple { static constexpr bool value = false; };
template <typename... Ts> struct is_tuple<std::tuple<Ts...>> {
  static constexpr bool value = true;
};
template <typename T> struct is_pair { static constexpr bool value = false; };
template <typename... Ts> struct is_pair<std::pair<Ts...>> {
  static constexpr bool value = true;
};

template <typename T> struct is_seq {
  static constexpr bool value = (is_vect<T>::value || is_array<T>::value ||
                                 is_tuple<T>::value || is_pair<T>::value);
  static std::string get_sequence_type() {
    if (!value) {
      return "non sequence type";
    }
    if (is_vect<T>::value) {
      return "std::vector";
    }
    if (is_array<T>::value) {
      return "std::array";
    }
    if (is_tuple<T>::value) {
      return "std::tuple";
    }
    if (is_pair<T>::value) {
      return "std::pair";
    }
  }
  static void test() {
    std::cout << "is_vect: " << is_vect<T>::value
              << ", is_array: " << is_array<T>::value
              << ", is_tuple: " << is_tuple<T>::value
              << ", is_pair: " << is_pair<T>::value << std::endl;
  }
};

template <typename T> struct string_rep_delim {
  static const std::map<char, char> brackets() { return {}; };
};
template <> struct string_rep_delim<std::string> {
  static const std::map<char, char> brackets() {
    return {{'\"', '\"'}, {'\'', '\''}};
  }
};
template <typename... Ts> struct string_rep_delim<std::vector<Ts...>> {
  static const std::map<char, char> brackets() {
    std::map<char, char> rtn{{'[', ']'}};
    for (auto const &br : string_rep_delim<
             typename std::vector<Ts...>::value_type>::brackets()) {
      rtn[br.first] = br.second;
    }
    return rtn;
  }
};
template <typename A, size_t N> struct string_rep_delim<std::array<A, N>> {
  static const std::map<char, char> brackets() {
    std::map<char, char> rtn{{'[', ']'}};
    for (auto const &br :
         string_rep_delim<typename std::array<A, N>::value_type>::brackets()) {
      rtn[br.first] = br.second;
    }
    return rtn;
  }
};
template <typename... Ts> struct string_rep_delim<std::pair<Ts...>> {
  static const std::map<char, char> brackets() {
    std::map<char, char> rtn{{'[', ']'}};
    for (auto const &br :
         string_rep_delim<typename std::pair<Ts...>::first_type>::brackets()) {
      rtn[br.first] = br.second;
    }
    for (auto const &br :
         string_rep_delim<typename std::pair<Ts...>::second_type>::brackets()) {
      rtn[br.first] = br.second;
    }
    return rtn;
  }
};

template <typename... Ts> struct string_rep_delim<std::tuple<Ts...>> {
  static const std::map<char, char> brackets() {
    std::map<char, char> rtn{{'[', ']'}};
    return rtn;
  }
};
} // namespace fhicl

#endif
