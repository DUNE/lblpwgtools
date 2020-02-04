#ifndef FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_HXX_SEEN
#define FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_HXX_SEEN

#include "string_parsers/exception.hxx"

#include <algorithm>
#include <iomanip>
#include <map>
#include <string>

namespace fhicl {
namespace string_parsers {

// trim from start (in place)
inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

inline bool well_bracket_wrapped(std::string str, char open_bracket = '{',
                                 char close_bracket = '}') {
  trim(str);
  if ((str.front() == open_bracket) || (str.back() == close_bracket)) {
    if (!((str.front() == open_bracket) && (str.back() == close_bracket))) {
      throw fhicl::string_parsers::mismatched_brackets()
          << "[ERROR]: Malformed bracketed string: " << std::quoted(str)
          << ", expected to find it bounded by " << open_bracket
          << close_bracket;
    }
    return true;
  }
  return false;
}

inline bool is_table(std::string const &str) {
  return well_bracket_wrapped(str);
}

inline bool is_sequence(std::string const &str) {
  return well_bracket_wrapped(str, '[', ']');
}

// #define FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_DEBUG
#ifdef FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_DEBUG
std::string indent = "";
void print_bracket_finder(std::string const &str, size_t begin = 0,
                          size_t end = 0) {
  std::cout << "[INFO]: " << indent << "\"" << str << "\"" << std::endl;
  std::cout << "         " << indent;
  for (size_t i = 0; i < begin; ++i) {
    std::cout << " ";
  }
  std::cout << "^";
  for (size_t i = begin + 1; i < end; ++i) {
    std::cout << " ";
  }
  std::cout << "^" << std::endl;
}
#endif

inline size_t find_matching_bracket(std::string const &str,
                                    char open_bracket = '{',
                                    char close_bracket = '}',
                                    size_t begin = 0) {

#ifdef FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_DEBUG
  std::map<char, char> matching_brackets = {{'(', ')'},   {'{', '}'},
                                            {'[', ']'},   {'<', '>'},
                                            {'\'', '\''}, {'\"', '\"'}};

  if (!matching_brackets.count(open_bracket)) {
    throw;
  } else if (matching_brackets[open_bracket] != close_bracket) {
    std::cout << "[ERROR]: expected " << open_bracket << " to match with "
              << matching_brackets[open_bracket] << " but found "
              << close_bracket << std::endl;
    throw;
  }
  if (str[begin] != open_bracket) {
    std::cout << "[ERROR]: Bad search starting position, expected to find \""
              << open_bracket << "\", but found \"" << str[begin] << "\""
              << std::endl;
    throw;
  }
#endif

  size_t next_match = str.find(close_bracket, begin + 1);
  size_t next_open = str.find(open_bracket, begin + 1);
  while (next_open < next_match) {
#ifdef FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_DEBUG
    indent += "  ";
#endif
    size_t next_open_match =
        find_matching_bracket(str, open_bracket, close_bracket, next_open);

#ifdef FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_DEBUG
    if (indent.size()) {
      indent.erase(indent.size() - 2);
    }
#endif

    next_match = str.find(close_bracket, next_open_match + 1);
    next_open = str.find(open_bracket, next_open_match + 1);
  }

  if (next_match == std::string::npos) {
    throw mismatched_brackets() << "[ERROR]: Failed to find matching bracket.";
  }
#ifdef FHICLCPP_SIMPLE_STRING_PARSERS_UTILITY_DEBUG
  print_bracket_finder(str, begin, next_match);
#endif
  return next_match;
}

inline std::string ensure_trailing_slash(std::string const &str) {
  if (!str.size()) {
    return str;
  }
  if (str.back() != '/') {
    return str + "/";
  }
  return str;
}

} // namespace string_parsers
} // namespace fhicl

#endif
