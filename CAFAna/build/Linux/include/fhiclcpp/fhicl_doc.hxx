#ifndef FHICLCPP_SIMPLE_FHICLCPP_FHICL_DOC_H_SEEN
#define FHICLCPP_SIMPLE_FHICLCPP_FHICL_DOC_H_SEEN

#include "fhiclcpp/exception.hxx"

#include "string_parsers/traits.hxx"
#include "string_parsers/utility.hxx"

#include "linedoc/doc.hxx"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Unix
#include <dirent.h>
#include <unistd.h>

namespace fhicl {

class fhicl_doc;

inline fhicl_doc read_doc(std::string const &filename);
inline linedoc::doc_line_point find_matching_bracket(
    fhicl_doc const &doc, char open_bracket = '{', char close_bracket = '}',
    linedoc::doc_line_point begin = linedoc::doc_line_point::begin());
inline std::vector<linedoc::doc_range>
get_list_elements(fhicl_doc const &doc, linedoc::doc_range range,
                  bool trim = false);

class fhicl_doc : public linedoc::doc {

  friend linedoc::doc_line_point
  find_matching_bracket(fhicl_doc const &, char, char, linedoc::doc_line_point);

  friend std::vector<linedoc::doc_range>
  get_list_elements(fhicl_doc const &, linedoc::doc_range, bool);

public:
  // #define DEBUG_RESOLVE_INCLUDES

  void resolve_includes(std::vector<std::string> include_chain = {}) {
    for (size_t i = 0; i < size(); ++i) {
#ifdef DEBUG_RESOLVE_INCLUDES
      std::cout << "[dri]: Checking line: " << std::quoted(at(i).characters)
                << " for include statements..." << std::endl;
#endif
      if (at(i).characters.find("#include \"") == 0) {
#ifdef DEBUG_RESOLVE_INCLUDES
        std::cout << "[dri]: Found one." << std::endl;
#endif
        size_t matchting_quote = at(i).characters.find_first_of('\"', 10);
        std::string inc_file_name =
            at(i).characters.substr(10, matchting_quote - 10);
        if (std::find(include_chain.begin(), include_chain.end(),
                      inc_file_name) != include_chain.end()) {

          throw include_loop()
              << "[ERROR]: Detected an include loop when trying to include: "
              << std::quoted(inc_file_name) << ". Current include chain: "
              << string_parsers::T2Str<std::vector<std::string>>(include_chain);
        }
        fhicl_doc inc;
        try {
          inc = read_doc(inc_file_name);
        } catch (file_does_not_exist &e) {
          throw file_does_not_exist()
              << "[ERROR]: When attempting to resolving include statement, "
                 "found at "
              << get_line_info(linedoc::doc_line_point{i, 0}) << " = "
              << std::quoted(get_line(linedoc::doc_line_point{i, 0}))
              << " caught exception:\n  --" << e.what() << "\n\n File "
              << std::quoted(inc_file_name)
              << " not found, or FHICL_FILE_PATH (="
              << (getenv("FHICL_FILE_PATH")
                      ? std::quoted(std::string(getenv("FHICL_FILE_PATH")))
                      : std::quoted(std::string("")))
              << ") improperly defined.";
        }
        include_chain.push_back(inc_file_name);
        inc.resolve_includes(include_chain);

#ifdef DEBUG_RESOLVE_INCLUDES
        std::cout << "[dri]: Loaded one with: " << inc.size() << " lines."
                  << std::endl;
#endif
#ifdef DEBUG_RESOLVE_INCLUDES
        std::cout << "[dri]: Before insert: " << size() << std::endl;
#endif
        // remove the #include line
        remove_line(i);
        // add the rest of the other file, file ID remap is done in insert.
        insert(std::move(inc), i);
#ifdef DEBUG_RESOLVE_INCLUDES
        std::cout << "[dri]: After insert: " << size() << std::endl;
#endif
      include_chain.pop_back();
      }
    }
  }

public:
};

// #define DEBUG_FIND_MATCHING_BRACKET

#ifdef DEBUG_FIND_MATCHING_BRACKET
std::string indent = "";
#endif

linedoc::doc_line_point find_matching_bracket(fhicl_doc const &doc,
                                              char open_bracket,
                                              char close_bracket,
                                              linedoc::doc_line_point begin) {

  if (doc.is_end(begin)) {
    throw internal_error()
        << "[ERROR]: Starting position " << begin
        << ", is equivalent to the end of the document: n_lines = "
        << doc.n_lines();
  }

  if (doc.get_char(begin) != open_bracket) {
    throw internal_error()
        << "[ERROR]: Bad search starting position, expected to find \""
        << open_bracket << "\", but found "
        << std::quoted(doc.get_line(begin, true)) << " from "
        << std::quoted(doc.get_line_info(begin));
  }

#ifdef DEBUG_FIND_MATCHING_BRACKET
  std::cout << indent << "Searching for " << close_bracket << " matching "
            << open_bracket << " from " << begin << std::endl;
#endif

  linedoc::doc_line_point search_from = doc.advance(begin);

#ifdef DEBUG_FIND_MATCHING_BRACKET
  std::cout << indent << "Next good char at " << search_from << " = "
            << std::quoted(doc.get_line(search_from, true)) << "." << std::endl;
#endif

  linedoc::doc_line_point next_match =
      doc.find_first_of(close_bracket, search_from);

#ifdef DEBUG_FIND_MATCHING_BRACKET
  std::cout << indent << "Next matching close at " << next_match << " = "
            << std::quoted(doc.get_line(next_match, true)) << "." << std::endl;
#endif

  linedoc::doc_line_point next_bracket_open;
  std::pair<char, char> brackets{'\0', '\0'};

  next_bracket_open = linedoc::doc_line_point::end();
  for (auto &bracket_types : std::vector<std::pair<char, char>>{
           {open_bracket, close_bracket}, {'\"', '\"'}}) {
    linedoc::doc_line_point bro =
        doc.find_first_of(bracket_types.first, search_from);
    if (doc.is_earlier(bro, next_bracket_open)) {
      brackets = bracket_types;
      next_bracket_open = bro;
    }
  }

#ifdef DEBUG_FIND_MATCHING_BRACKET
  if (!doc.is_end(next_bracket_open)) {
    std::cout << indent << "Found opening bracket of type " << brackets.first
              << " at " << next_bracket_open << " = "
              << std::quoted(doc.get_line(next_bracket_open, true)) << "."
              << std::endl;
  }
#endif

  while (doc.is_earlier(next_bracket_open, next_match)) {

#ifdef DEBUG_FIND_MATCHING_BRACKET
    std::cout << indent
              << "Searching for matching close bracket: " << brackets.second
              << " from " << next_bracket_open << " = "
              << std::quoted(doc.get_line(next_bracket_open, true)) << "."
              << std::endl;
    indent = indent + "  ";
#endif

    linedoc::doc_line_point next_bracket_open_match = find_matching_bracket(
        doc, brackets.first, brackets.second, next_bracket_open);

#ifdef DEBUG_FIND_MATCHING_BRACKET
    indent = indent.substr(2);
    std::cout << indent << "Found closing bracket of type " << brackets.second
              << " at " << next_bracket_open_match << " = "
              << std::quoted(doc.get_line(next_bracket_open_match, true)) << "."
              << std::endl;
#endif

    search_from = doc.advance(next_bracket_open_match);

#ifdef DEBUG_FIND_MATCHING_BRACKET
    std::cout << indent << "Next good char "
              << std::quoted(doc.get_line(search_from, true)) << " at "
              << search_from << std::endl;
#endif

    next_match = doc.find_first_of(close_bracket, search_from);

#ifdef DEBUG_FIND_MATCHING_BRACKET
    std::cout << indent << "Found main closing bracket of type "
              << close_bracket << " at " << next_match << " = "
              << std::quoted(doc.get_line(next_match, true)) << "."
              << std::endl;
#endif

    next_bracket_open = linedoc::doc_line_point::end();
    for (auto &bracket_types : std::vector<std::pair<char, char>>{
             {open_bracket, close_bracket}, {'\"', '\"'}}) {
      linedoc::doc_line_point bro =
          doc.find_first_of(bracket_types.first, search_from);
      if (doc.is_earlier(bro, next_bracket_open)) {
        brackets = bracket_types;
        next_bracket_open = bro;
      }
    }

#ifdef DEBUG_FIND_MATCHING_BRACKET
    if (!doc.is_end(next_bracket_open)) {
      std::cout << indent << "Found opening bracket of type " << brackets.first
                << " at " << next_bracket_open << " = "
                << std::quoted(doc.get_line(next_bracket_open, true))
                << " (Next matching closing bracket at: " << next_match << ")."
                << std::endl;
    }
#endif
  }

  if (doc.is_end(next_match)) {
    throw malformed_document()
        << "[ERROR]: Failed to find matching bracket to " << begin << ", from "
        << doc.get_line_info(begin);
  }

#ifdef DEBUG_FIND_MATCHING_BRACKET
  std::cout << indent << "Returning matching close " << doc.get_char(next_match)
            << " at " << next_match << " = \"" << doc.get_line(next_match, true)
            << "\"." << std::endl;
#endif
  return next_match;
}

// #define DEBUG_GET_LIST_ELEMENTS

std::vector<linedoc::doc_range>
get_list_elements(fhicl_doc const &doc, linedoc::doc_range range, bool trim) {

#ifdef DEBUG_GET_LIST_ELEMENTS
  std::cout << "Begin at: " << range.begin << " -- "
            << std::quoted(doc.get_line(range.begin, true)) << std::endl;
#endif
#ifdef DEBUG_GET_LIST_ELEMENTS
  std::cout << "Go until: " << range.end << " -- "
            << std::quoted(doc.get_line(range.end, true)) << std::endl;
#endif

  linedoc::doc_line_point nextOccurence = range.begin;
  linedoc::doc_line_point searchZero = range.begin;
  std::vector<linedoc::doc_range> outV;

  static const std::map<char, char> type_care_brackets =
      string_rep_delim<std::vector<std::string>>::brackets();

  while (doc.is_earlier(searchZero, range.end)) {

    linedoc::doc_line_point next_comment =
        doc.find_first_of('#', searchZero, range.end);

#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  Next comment character at " << next_comment << " -- "
              << std::quoted(doc.get_line(next_comment, true)) << std::endl;
#endif

    linedoc::doc_line_point next_non_ws =
        doc.find_first_not_of(" \t\n", searchZero, range.end);

#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  Next non-ws character at " << next_non_ws << " -- "
              << std::quoted(doc.get_line(next_non_ws, true)) << std::endl;
#endif

    if (doc.are_equivalent(next_comment, next_non_ws)) {
      // If the next comment character is the next non-whitespace character,
      // push pointer to the end of the line.
      searchZero = doc.find_first_of('\n', next_comment);

#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << "  Moved pointer to end of line comment " << searchZero
                << " -- " << std::quoted(doc.get_line(searchZero, true))
                << std::endl;
#endif

      continue;
    }

#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  Searching for next comma after: " << searchZero << " -- "
              << std::quoted(doc.get_line(searchZero, true)) << std::endl;
#endif
    nextOccurence = doc.find_first_of(',', searchZero);

#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  Next comma at: " << nextOccurence << " -- "
              << std::quoted(doc.get_line(nextOccurence, true)) << std::endl;
#endif

    linedoc::doc_line_point bracketSearchZero = searchZero;

#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  Searching for brackets from : " << searchZero << " -- "
              << std::quoted(doc.get_line(searchZero, true)) << std::endl;
#endif

    while ((!doc.is_end(nextOccurence)) && type_care_brackets.size()) {

      linedoc::doc_line_point nextBracket = nextOccurence;
      std::pair<char, char> brackets{'\0', '\0'};
      for (auto const &bracket_pair : type_care_brackets) {
        linedoc::doc_line_point f =
            doc.find_first_of(bracket_pair.first, bracketSearchZero);
#ifdef DEBUG_GET_LIST_ELEMENTS
        std::cout << "  Next bracket of type " << bracket_pair.first
                  << " is at: " << f << " -- "
                  << std::quoted(doc.get_line(f, true)) << std::endl;
#endif
        if (doc.is_earlier(f, nextBracket)) {
#ifdef DEBUG_GET_LIST_ELEMENTS
          std::cout << "  + This is now the closest bracket at: " << f << " -- "
                    << std::quoted(doc.get_line(f, true)) << std::endl;
#endif
          nextBracket = f;
          brackets = bracket_pair;
        }
      }

#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << "  Next bracket at: " << nextBracket
                << ", next comma at: " << nextOccurence << std::endl;
#endif
      if (doc.is_later(nextBracket,
                       nextOccurence)) { // The next bracket that we care about
// is after the next comma
#ifdef DEBUG_GET_LIST_ELEMENTS
        std::cout << "  Breaking." << std::endl;
#endif
        break;
      } else if (doc.is_earlier(
                     nextBracket,
                     nextOccurence)) { // The next bracket comes before the next
                                       // comma, the comma could be contained
                                       // within that bracket.
#ifdef DEBUG_GET_LIST_ELEMENTS
        std::cout << "  Next bracket is worrying, must check for matching "
                     "bracket of type: "
                  << brackets.second << std::endl;
#endif
        linedoc::doc_line_point matching_bracket = find_matching_bracket(
            doc, brackets.first, brackets.second, nextBracket);

        if (((brackets.first == '\"') || (brackets.first == '\'')) &&
            !doc.is_earlier(matching_bracket, nextBracket.get_EOL())) {
          throw malformed_document()
              << "[ERROR]: When searching for matching quote to the one "
                 "found "
                 "at "
              << nextBracket << " on line "
              << std::quoted(doc.get_line(nextBracket, true))
              << " in document: " << std::quoted(doc.get_line_info(nextBracket))
              << " the end of the line was reached. Quoted strings cannot "
                 "span "
                 "multiple lines.";
        }

#ifdef DEBUG_GET_LIST_ELEMENTS
        std::cout << "  Found matching bracket at : " << matching_bracket
                  << " -- " << std::quoted(doc.get_line(matching_bracket, true))
                  << ", next comma is at: " << nextOccurence << std::endl;
#endif
        if (doc.is_later(
                matching_bracket,
                nextOccurence)) { // The matching bracket occurs after the next
                                  // comma, therefore we should skip that comma
                                  // and look for the next one.
#ifdef DEBUG_GET_LIST_ELEMENTS
          std::cout << "  Skipping this comma..." << std::endl;
#endif
          bracketSearchZero = doc.advance(matching_bracket);
#ifdef DEBUG_GET_LIST_ELEMENTS
          std::cout << " Continuing bracket search from: " << bracketSearchZero
                    << " -- "
                    << std::quoted(doc.get_line(bracketSearchZero, true))
                    << std::endl;
#endif
          nextOccurence = doc.find_first_of(',', bracketSearchZero);
#ifdef DEBUG_GET_LIST_ELEMENTS
          std::cout << " Updated next comma occurence to: " << nextOccurence
                    << " -- " << std::quoted(doc.get_line(nextOccurence, true))
                    << std::endl;
#endif
          if (doc.are_equivalent(doc.advance(matching_bracket),
                                 nextOccurence)) {
#ifdef DEBUG_GET_LIST_ELEMENTS
            std::cout << "  The bracket closes on the character before the "
                         "next comma, short circuiting."
                      << std::endl;
#endif
            break;
          }
          continue;

        } else if (doc.is_earlier(matching_bracket,
                                  nextOccurence)) { // That bracket ends
                                                    // before the next comma,
          // but there could be another one.
          if (doc.are_equivalent(doc.advance(matching_bracket),
                                 nextOccurence)) {
#ifdef DEBUG_GET_LIST_ELEMENTS
            std::cout << "  The bracket closes on the character before the "
                         "next comma, short circuiting."
                      << std::endl;
#endif
            break;
          }
          bracketSearchZero = doc.advance(matching_bracket);
#ifdef DEBUG_GET_LIST_ELEMENTS
          std::cout << "  The bracket closes before the next comma, searching "
                       "for any new worrying brackets from: "
                    << bracketSearchZero << std::endl;
#endif
          continue;
        } else if (doc.is_end(matching_bracket)) {
          throw malformed_document()
              << "[ERROR]: When searching for closing bracket that matches "
                 "the "
                 "opening one found at: "
              << nextBracket << " on line "
              << std::quoted(doc.get_line(nextBracket, true)) << " in document "
              << std::quoted(doc.get_line_info(nextBracket))
              << " hit the end of document.";

        } else {
          throw bizare_error()
              << "[ERROR]: Matching bracket occured at the same place as the "
                 "next comma, this shouldn't have happened.";
        }
      } else {
#ifdef DEBUG_GET_LIST_ELEMENTS
        std::cout << "  The next bracket opens after the next comma."
                  << std::endl;
#endif
        break;
      }
    }

#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  The next comma found, which delimits a list item was "
                 "determined to be at: "
              << nextOccurence << std::endl;
#endif

    if (doc.is_later(nextOccurence, range.end)) {
      nextOccurence = range.end;
#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << " The next comma ran passed the end of the allowed document "
                   "region. Ending list element search here."
                << std::endl;
#endif
    }
    outV.push_back({doc.validate_line_point(searchZero),
                    doc.validate_line_point(nextOccurence)});
#ifdef DEBUG_GET_LIST_ELEMENTS
    std::cout << "  Element determined as: " << searchZero << " -- "
              << nextOccurence << " = "
              << std::quoted(doc.substr(searchZero, nextOccurence))
              << std::endl;
#endif
    searchZero = doc.advance(nextOccurence);
  }
  if (trim) {
    for (linedoc::doc_range &r : outV) {
#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << "Range begin = " << r.begin << " -- "
                << std::quoted(doc.get_line(r.begin, true)) << std::endl;
#endif
#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << "  Range end = " << r.end << " -- "
                << std::quoted(doc.get_line(r.end, true)) << std::endl;
#endif

      linedoc::doc_line_point nb = doc.find_first_not_of(" \n", r.begin, r.end);
      if (!doc.is_end(nb)) {
        r.begin = nb;
      } else { // If there's nothing, set this element to empty.
        r.begin = r.end;
      }
      linedoc::doc_line_point ne = doc.find_last_not_of(" \n", r.begin, r.end);
      if (!doc.is_end(ne)) {
        r.end = doc.advance(ne);
      }
#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << "  Trimmed begin = " << r.begin << " -- "
                << std::quoted(doc.get_line(r.begin, true)) << std::endl;
#endif
#ifdef DEBUG_GET_LIST_ELEMENTS
      std::cout << "  Trimmed end = " << r.end << " -- "
                << std::quoted(doc.get_line(r.end, true)) << std::endl;
#endif
      if (doc.is_earlier(r.end, r.begin)) {
        throw bizare_error()
            << "[ERROR]: After trimming, list item ends "
               "before beggining: begin = "
            << std::quoted(doc.get_line(r.begin, true))
            << ", end: " << std::quoted(doc.get_line(r.end, true));
      }
    }
  }
  return outV;
}

// #define DEBUG_OPEN_FHICL_FILE

inline std::unique_ptr<std::ifstream>
open_fhicl_file(std::string const &filename) {

#ifdef DEBUG_OPEN_FHICL_FILE
  std::cout << "[open_fhicl_file]: Trying to resolve " << std::quoted(filename)
            << std::endl;
#endif

  if (filename.find_last_of('/') !=
      std::string::npos) { // If there are slashes, assume that it is a
                           // relative path.
#ifdef DEBUG_OPEN_FHICL_FILE
    std::cout << "[open_fhicl_file]: Found a forward slash, assuming a "
                 "relative path."
              << std::endl;
#endif
    return std::make_unique<std::ifstream>(filename);
  }

  char const *fhicl_file_path = getenv("FHICL_FILE_PATH");
  if (!fhicl_file_path) { // Try and open the file anyway assuming that it is
// relative
#ifdef DEBUG_OPEN_FHICL_FILE
    std::cout << "[open_fhicl_file]: FHICL_FILE_PATH is not defined, trying to "
                 "treat it as a local filename."
              << std::endl;
#endif
    return std::make_unique<std::ifstream>(filename);
  }

  for (std::string const &path : string_parsers::ParseToVect<std::string>(
           fhicl_file_path, ":", false, true)) {
#ifdef DEBUG_OPEN_FHICL_FILE
    std::cout << "[open_fhicl_file]: Searching directory from FHICL_FILE_PATH: "
              << std::quoted(path) << std::endl;
#endif
    DIR *dir = opendir(path.c_str());
    struct dirent *ent;
    if (dir != NULL) {
      while ((ent = readdir(dir)) != NULL) {
#ifdef DEBUG_OPEN_FHICL_FILE
        std::cout << "[open_fhicl_file]: Checking entry: "
                  << std::quoted(ent->d_name) << std::endl;
#endif
        if (std::string(ent->d_name) == filename) {
#ifdef DEBUG_OPEN_FHICL_FILE
          std::cout << "[open_fhicl_file]: Attempting to open : "
                    << std::quoted(string_parsers::ensure_trailing_slash(path) +
                                   filename)
                    << std::endl;
#endif
          return std::make_unique<std::ifstream>(
              string_parsers::ensure_trailing_slash(path) + filename);
        }
      }
      closedir(dir);
    } else { // Couldn't open directory
      std::cout << "[WARN]: Failed to search directory: " << std::quoted(path)
                << " found in FHICL_FILE_PATH. opendir failed with error: "
                << std::quoted(std::strerror(errno)) << std::endl;
    }
  }
  return nullptr;
}

inline fhicl_doc read_doc(std::string const &filename) {
  std::unique_ptr<std::ifstream> ifs = open_fhicl_file(filename);
  if (!ifs || !ifs->good()) {
    throw file_does_not_exist()
        << "[ERROR]: File: " << std::quoted(filename)
        << " could not be opened for reading. (N.B. files in the working "
           "directory must be explicitly qualified with \"./\" to avoid "
           "confusion with files found in FHICL_FILE_PATH)";
  }
  std::string line;
  fhicl_doc doc;
  size_t ctr = 0;
  while (std::getline(*ifs, line)) {
    string_parsers::trim(line);
    doc.push_back(line, filename, ctr);
    ctr++;
  }
  return doc;
}

} // namespace fhicl
#endif
