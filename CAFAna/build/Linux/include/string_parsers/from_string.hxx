#ifndef FHICLCPP_SIMPLE_STRING_PARSERS_FROM_STRING_HXX_SEEN
#define FHICLCPP_SIMPLE_STRING_PARSERS_FROM_STRING_HXX_SEEN

#include "string_parsers/traits.hxx"
#include "string_parsers/utility.hxx"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace fhicl {

namespace string_parsers {

template <typename T>
inline
    typename std::enable_if<!is_seq<T>::value && std::is_arithmetic<T>::value,
                            T>::type
    str2T(std::string const &str) {
  if (!str.size()) {
    return T{};
  }
  std::string cpy = str;
  trim(cpy);
  if (cpy == "@nil") {
    return T{};
  }
  std::stringstream stream("");

  if (cpy.front() == '0' && (cpy.size() > 1)) {
    if ((cpy[1] == 'x') || (cpy[1] == 'X')) {
      stream << std::hex;
      cpy = cpy.substr(2);
      stream << cpy;
      unsigned int d;
      stream >> d;
      return d;
    }
    if ((cpy[1] == 'b') || (cpy[1] == 'B')) {
      size_t ptr = 2;
      long rtn = 0;
      bool leadingZeros = true;
      for (; ptr < cpy.size(); ++ptr) {
        if (cpy[ptr] == '0') {
          if (leadingZeros) {
            continue;
          }
          rtn = rtn << 1;
        } else if (cpy[ptr] == '0') {
          rtn = rtn << 1;
          rtn += 1;
        } else {
          throw parser_fail() << "[ERROR]: Failed to parse " << std::quoted(str)
                              << " as a binary constant.";
        }
      }
      return rtn;
    }
  }
  // Translating exponential notation for non-floating types as stringstream
  // baulks.
  if (cpy.find("E-") != std::string::npos) {
    // std::cout << "CONVERTED -: " << cpy << std::endl;
    int exp = str2T<size_t>(cpy.substr(cpy.find("E-") + 2));
    cpy = cpy.substr(0, cpy.find("E-"));

    bool isneg = cpy.front() == '-';
    if (isneg) {
      cpy.erase(std::remove(cpy.begin(), cpy.end(), '-'), cpy.end());
    }
    // Is there already a decimal place?
    int dec_pos = cpy.find(".");
    if (size_t(dec_pos) == std::string::npos) {
      dec_pos = cpy.length();
    } else {
      cpy.erase(std::remove(cpy.begin(), cpy.end(), '.'), cpy.end());
    }
    int move_dec_pos = dec_pos - exp;
    if (move_dec_pos > 0) {
      cpy.insert(cpy.begin() + move_dec_pos, '.');
    } else {
      // std::cout << "\tprefixing -: " << exp << " - " << dec_pos << " = "
      //           << (exp - dec_pos) << " 0s" << std::endl;

      for (int i = 0; i < (exp - dec_pos); ++i) {
        cpy.insert(cpy.begin(), '0');
      }
      cpy.insert(cpy.begin(), '.');
    }

    if (isneg) {
      cpy.insert(cpy.begin(), '-');
    }
    // std::cout << "\tTo -: " << cpy << std::endl;
  } else if (cpy.find("E") != std::string::npos) {
    // std::cout << "CONVERTED +: " << cpy << std::endl;
    int exp = str2T<size_t>(cpy.substr(cpy.find("E") + 1));
    cpy = cpy.substr(0, cpy.find("E"));

    bool isneg = cpy.front() == '-';
    if (isneg) {
      cpy.erase(std::remove(cpy.begin(), cpy.end(), '-'), cpy.end());
    }
    // Is there already a decimal place?
    int dec_pos = cpy.find(".");
    if (size_t(dec_pos) == std::string::npos) {
      dec_pos = cpy.length();
    } else {
      cpy.erase(std::remove(cpy.begin(), cpy.end(), '.'), cpy.end());
    }
    int move_dec_pos = dec_pos + exp;
    if (size_t(move_dec_pos) < cpy.length()) {
      cpy.insert(cpy.begin() + move_dec_pos, '.');
    } else {
      int extra = move_dec_pos - cpy.length();
      // std::cout << "\tpostfixing +: " << extra << " 0s" << std::endl;
      for (int i = 0; i < extra; ++i) {
        cpy.insert(cpy.end(), '0');
      }
    }

    if (isneg) {
      cpy.insert(cpy.begin(), '-');
    }
    // std::cout << "\tTo +: " << cpy << std::endl;
  }

  stream << cpy;
  T d;
  stream >> d;

  if (stream.fail()) {
    throw parser_fail() << "[ERROR]: Failed to parse string: "
                        << std::quoted(str) << " as requested type.";
  }

  return d;
}

template <typename T>
inline
    typename std::enable_if<!is_seq<T>::value && !std::is_arithmetic<T>::value,
                            T>::type
    str2T(std::string const &str) {
  if (!str.size()) {
    return T{};
  }
  std::string cpy = str;
  trim(cpy);
  if (cpy == "@nil") {
    return T{};
  }
  std::stringstream stream("");

  stream << cpy;
  T d;
  stream >> d;

  if (stream.fail()) {
    throw parser_fail() << "[ERROR]: Failed to parse string: "
                        << std::quoted(str) << " as requested type.";
  }

  return d;
}

template <> inline bool str2T<bool>(std::string const &str) {
  if ((str == "true") || (str == "True") || (str == "TRUE") || (str == "1")) {
    return true;
  }

  if ((str == "false") || (str == "False") || (str == "FALSE") ||
      (str == "0")) {
    return false;
  }

  std::istringstream stream(str);
  bool d;
  stream >> d;

  if (stream.fail()) {
    throw parser_fail() << "[ERROR]: Failed to parse string: " << str
                        << " as boolean.";
  }

  return d;
}

template <> inline std::string str2T<std::string>(std::string const &str) {
  std::string cpy = str;
  trim(cpy);
  if (well_bracket_wrapped(cpy, '\"', '\"') ||
      well_bracket_wrapped(cpy, '\'', '\'')) {
    cpy = cpy.substr(1, cpy.size() - 2);
  }
  return cpy;
}

template <typename T>
inline typename std::enable_if<is_vect<T>::value, T>::type
str2T(std::string const &str);
template <typename T>
inline typename std::enable_if<is_pair<T>::value, T>::type
str2T(std::string const &str);
template <typename T>
inline typename std::enable_if<is_array<T>::value, T>::type
str2T(std::string const &str);
template <typename T>
inline typename std::enable_if<is_tuple<T>::value, T>::type
str2T(std::string const &str);

template <typename T>
inline std::vector<T> ParseToVect(std::string const &inp,
                                  std::vector<std::string> const &delims,
                                  bool PushEmpty, bool trimInput,
                                  std::map<char, char> extra_care_brackets = {
                                      {'\"', '\"'}, {'\'', '\''}}) {
  std::string inpCpy = inp;
  if (trimInput) {
    trim(inpCpy);
  }
  size_t nextOccurence = 0;
  size_t prevOccurence = 0;
  std::vector<T> outV;
  bool AtEnd = false;
  static const std::map<char, char> type_care_brackets =
      string_rep_delim<std::vector<T>>::brackets();
  while (!AtEnd) {
    std::string delim;
    nextOccurence = std::string::npos;
    for (auto const &d : delims) {
      size_t delim_NO = inpCpy.find(d, prevOccurence);
      if (delim_NO < nextOccurence) {
        delim = d;
        nextOccurence = delim_NO;
      }
    }

    size_t bracketSearchZero = prevOccurence;
    while ((nextOccurence != std::string::npos) && type_care_brackets.size()) {
      size_t nextBracket = std::string::npos;
      std::pair<char, char> brackets{'\0', '\0'};
      for (auto const &bracket_pair : type_care_brackets) {
        size_t f = inpCpy.find_first_of(bracket_pair.first, bracketSearchZero);
        if (f < nextBracket) {
          nextBracket = f;
          brackets = bracket_pair;
        }
      }
      for (auto const &bracket_pair : extra_care_brackets) {
        size_t f = inpCpy.find_first_of(bracket_pair.first, bracketSearchZero);
        if (f < nextBracket) {
          nextBracket = f;
          brackets = bracket_pair;
        }
      }

      if (nextBracket > nextOccurence) {
        break;
      } else if (nextBracket < nextOccurence) {
        size_t matching_bracket = find_matching_bracket(
            inpCpy, brackets.first, brackets.second, nextBracket);
        if (matching_bracket > nextOccurence) {
          bracketSearchZero = matching_bracket + 1;
          nextOccurence = std::string::npos;
          for (auto const &d : delims) {
            size_t delim_NO = inpCpy.find(d, bracketSearchZero);
            if (delim_NO < nextOccurence) {
              delim = d;
              nextOccurence = delim_NO;
            }
          }
          continue;
        } else if (matching_bracket < nextOccurence) {
          bracketSearchZero = matching_bracket + 1;
          continue;
        } else {
          throw parser_fail()
              << "[ERROR]: Matching bracket is the same as the next delimiter, "
                 "delimiters including any of [{,},[,],<,>,(,),\",\'] "
                 "should be avoided.";
        }
      } else if (nextBracket != std::string::npos) {
        throw parser_fail()
            << "[ERROR]: Next bracket is the same as the next delimiter ("
            << nextBracket << " = \'" << inpCpy[nextBracket]
            << "\'), "
               "delimiters including any of [{,},[,],<,>,(,),\",\'] "
               "should be avoided.";
      } else {
        break;
      }
    }

    if (nextOccurence == std::string::npos) {
      if (prevOccurence == inpCpy.length()) {
        break;
      }
      AtEnd = true;
    }
    if (PushEmpty || (nextOccurence != prevOccurence)) {
      outV.push_back(str2T<T>(
          inpCpy.substr(prevOccurence, (nextOccurence - prevOccurence))));
    }
    prevOccurence = nextOccurence + delim.size();
  }
  return outV;
}

template <typename T>
inline std::vector<T> ParseToVect(std::string const &inp,
                                  std::string const &delim, bool PushEmpty,
                                  bool trimInput,
                                  std::map<char, char> extra_care_brackets = {
                                      {'\"', '\"'}, {'\'', '\''}}) {
  return ParseToVect<T>(inp, std::vector<std::string>{delim}, PushEmpty,
                        trimInput, extra_care_brackets);
}

template <typename T>
inline typename std::enable_if<is_vect<T>::value, T>::type
str2T(std::string const &str) {
  std::string tstr = str;
  trim(tstr);
  if (!tstr.size()) {
    throw parser_fail()
        << "[ERROR]: Attempted to parse empty string as a vector of types.";
  }
  if (!well_bracket_wrapped(tstr, '[', ']')) {
    throw parser_fail()
        << "[ERROR]: Attempted to parse a non-array-like string ("
        << std::quoted(str)
        << ") as a "
           "vector of types.";

  } else {
    tstr = tstr.substr(1, tstr.size() - 2);
  }

  return ParseToVect<typename T::value_type>(tstr, ",", true, true);
}

template <typename T>
inline typename std::enable_if<is_pair<T>::value, T>::type
str2T(std::string const &str) {
  std::vector<std::string> str_parse = str2T<std::vector<std::string>>(str);

  if (str_parse.size() != 2) {
    throw wrong_number_of_elements()
        << "[ERROR]: When parsing " << std::quoted(str) << " to a pair, found "
        << str_parse.size() << " elements, but expected 2.";
  }

  return {str2T<typename T::first_type>(str_parse.front()),
          str2T<typename T::second_type>(str_parse.back())};
}

template <typename T>
inline typename std::enable_if<is_array<T>::value, T>::type
str2T(std::string const &str) {
  std::vector<typename T::value_type> str_parse =
      str2T<std::vector<typename T::value_type>>(str);

  if (str_parse.size() != std::tuple_size<T>::value) {
    throw wrong_number_of_elements()
        << "[ERROR]: When parsing " << std::quoted(str)
        << " to an array, found " << str_parse.size()
        << " elements, but expected " << std::tuple_size<T>::value;
  }

  T rtn;
  for (size_t i = 0; i < std::tuple_size<T>::value; ++i) {
    rtn[i] = str_parse[i];
  }
  return rtn;
}

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
assign_tuple(std::tuple<Tp...> &, std::vector<std::string> const &) {}

template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if <
    I<sizeof...(Tp), void>::type
    assign_tuple(std::tuple<Tp...> &t,
                 std::vector<std::string> const &str_parse) {
  std::get<I>(t) =
      str2T<typename std::tuple_element<I, std::tuple<Tp...>>::type>(
          str_parse[I]);
  assign_tuple<I + 1, Tp...>(t, str_parse);
}

template <typename T>
inline typename std::enable_if<is_tuple<T>::value, T>::type
str2T(std::string const &str) {
  std::vector<std::string> str_parse = str2T<std::vector<std::string>>(str);

  if (str_parse.size() != std::tuple_size<T>::value) {
    throw wrong_number_of_elements()
        << "[ERROR]: When parsing " << std::quoted(str) << " to a tuple, found "
        << str_parse.size() << " elements, but expected "
        << std::tuple_size<T>::value;
  }

  T rtn;
  assign_tuple(rtn, str_parse);
  return rtn;
}

} // namespace string_parsers
} // namespace fhicl
#endif
