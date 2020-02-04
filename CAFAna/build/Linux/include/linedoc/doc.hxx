#ifndef LINEDOC_DOC_HXX_SEEN
#define LINEDOC_DOC_HXX_SEEN

#ifndef LINEDOC_PROTECTED
#define LINEDOC_PROTECTED protected
#endif

#include "linedoc/doc_line.hxx"
#include "linedoc/doc_line_point.hxx"
#include "linedoc/doc_range.hxx"

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace linedoc {

template <typename T> class doc_ : LINEDOC_PROTECTED std::vector<doc_line_<T>> {
  LINEDOC_PROTECTED :

      std::vector<std::basic_string<T>>
          filenames;

  /// Check whether line_no or character valid, update to special values if not.
  inline doc_line_point_<T> validate_line_point(doc_line_point_<T>) const;

  inline size_t get_filename_id(std::basic_string<T> const &filename = "");

  void tidy_filenames();

public:
  /// Move to the next character
  ///
  ///\note newline counts as a character
  ///
  ///\note If n = 0, do nothing.
  inline doc_line_point_<T> advance(doc_line_point_<T>, size_t n = 1) const;

  /// Move to the previous character
  ///
  ///\note newline counts as a character
  ///
  ///\note If n = 0, do nothing.
  inline doc_line_point_<T> rewind(doc_line_point_<T>, size_t n = 1) const;

  /// Move to the beginning of the line n lines after this.
  ///
  ///\note If n = 0, do nothing.
  inline doc_line_point_<T> advance_line(doc_line_point_<T>,
                                         size_t n = 1) const;
  /// Move to the end of the previous line
  ///
  ///\note If n = 0, do nothing.
  inline doc_line_point_<T> rewind_line(doc_line_point_<T>, size_t n = 1) const;

  inline bool line_is_empty(doc_line_point_<T>) const;

  inline T get_char(doc_line_point_<T>) const;

  /// First character of the document
  inline doc_line_point_<T> begin() const;
  /// Last character of the document
  inline doc_line_point_<T> end() const;

  /// Whether two line_points are equivalent
  inline bool are_equivalent(doc_line_point_<T>, doc_line_point_<T>) const;

  /// Whether one line_point is earlier than another
  inline bool is_earlier(doc_line_point_<T>, doc_line_point_<T>) const;

  /// Whether one line_point is later than another
  inline bool is_later(doc_line_point_<T>, doc_line_point_<T>) const;

  /// Get the earlier of two doc_line_points
  inline doc_line_point_<T> get_earlier(doc_line_point_<T>,
                                        doc_line_point_<T>) const;
  /// Get the later of two doc_line_points
  inline doc_line_point_<T> get_later(doc_line_point_<T>,
                                      doc_line_point_<T>) const;

  /// Whether two line_points occur on equivalent lines.
  inline bool same_line(doc_line_point_<T>, doc_line_point_<T>) const;

  /// Whether a line_point is equivalent to the beginning of the document.
  inline bool is_begin(doc_line_point_<T>) const;
  /// Whether a line_point is equivalent to the end of the document.
  inline bool is_end(doc_line_point_<T>) const;

  /// The number of lines in this document
  inline size_t n_lines() const;

  /// Find the first occurence of a character in the passed basic_string
  inline doc_line_point_<T>
  find_first_of(std::basic_string<T> const &,
                doc_line_point_<T> start = doc_line_point_<T>::begin(),
                doc_line_point_<T> end = doc_line_point_<T>::end()) const;
  /// Find the first occurence of a character
  inline doc_line_point_<T>
  find_first_of(T, doc_line_point_<T> start = doc_line_point_<T>::begin(),
                doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the first occurence of a character not in the passed basic_string
  inline doc_line_point_<T>
  find_first_not_of(std::basic_string<T> const &,
                    doc_line_point_<T> start = doc_line_point_<T>::begin(),
                    doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the first occurence of a character
  inline doc_line_point_<T>
  find_first_not_of(T, doc_line_point_<T> start = doc_line_point_<T>::begin(),
                    doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the last occurence of a character in the passed basic_string
  inline doc_line_point_<T>
  find_last_of(std::basic_string<T> const &,
               doc_line_point_<T> start = doc_line_point_<T>::begin(),
               doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the last occurence of a character in the passed basic_string
  inline doc_line_point_<T>
  find_last_of(T, doc_line_point_<T> start = doc_line_point_<T>::begin(),
               doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the last occurence of a character not in the passed basic_string
  inline doc_line_point_<T>
  find_last_not_of(std::basic_string<T> const &,
                   doc_line_point_<T> start = doc_line_point_<T>::begin(),
                   doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Find the last occurence of a character not in the passed basic_string
  inline doc_line_point_<T>
  find_last_not_of(T, doc_line_point_<T> start = doc_line_point_<T>::begin(),
                   doc_line_point_<T> end = doc_line_point_<T>::end()) const;

  /// Get a document spanning the passed range
  inline doc_<T>
  subdoc(doc_line_point_<T> begin = doc_line_point_<T>::begin(),
         doc_line_point_<T> end = doc_line_point_<T>::end()) const;
  inline doc_<T> subdoc(doc_range_<T>) const;
  /// Get a string spanning the passed range
  ///
  /// \note newlines are converted to a single space
  inline std::basic_string<T>
  substr(doc_line_point_<T> begin = doc_line_point_<T>::begin(),
         doc_line_point_<T> end = doc_line_point_<T>::end()) const;
  inline std::basic_string<T> substr(doc_range_<T> rng) const;

  inline std::basic_string<T> get_line(doc_line_point_<T>,
                                       bool highlight = true) const;
  inline std::basic_string<T> get_line(size_t) const;

  inline std::basic_string<T> get_line_info(doc_line_point_<T>) const;
  inline std::basic_string<T> get_line_info(size_t) const;

  inline void insert(doc_<T> const &, size_t line = 0);
  inline void insert(doc_<T> &&, size_t line = 0);

  inline void remove_line(doc_line_point_<T>);
  inline void remove_line(size_t line_no);

  inline void push_back(std::basic_string<T> const &line,
                        std::basic_string<T> const &filename = "",
                        size_t line_no = std::numeric_limits<size_t>::max());

  inline void push_back(std::basic_string<T> &&,
                        std::basic_string<T> const &filename = "",
                        size_t line_no = std::numeric_limits<size_t>::max());
};

typedef doc_<char> doc;
typedef doc_<wchar_t> wdoc;

template <typename T>
doc_line_point_<T> doc_<T>::validate_line_point(doc_line_point_<T> lp) const {
  // If off the end, return the EOL on the last line
  size_t size = std::vector<doc_line_<T>>::size();
  if (lp.line_no >= size) {
    if (!size) {
      return doc_line_point_<T>{std::numeric_limits<size_t>::max(),
                                std::basic_string<T>::npos};
    }
    return doc_line_point_<T>{size - 1, std::basic_string<T>::npos};
  } else if (std::vector<doc_line_<T>>::at(lp.line_no).is_EOL(lp.character)) {
    lp.character = std::string::npos;
  }
  return lp;
}

template <typename T>
doc_line_point_<T> doc_<T>::advance(doc_line_point_<T> lp, size_t n) const {
  lp = validate_line_point(lp);
  if (!n) {
    return lp;
  }

  size_t n_left = n;

  while (n_left) {
    if (lp >= end()) {
      return lp;
    }
    size_t n_in_line =
        std::vector<doc_line_<T>>::at(lp.line_no).size() - lp.character;
    if (n_left <= n_in_line) {
      lp.character += n_left;
      break;
    }
    n_left -= n_in_line;
    lp.character = std::basic_string<T>::npos;
    if (n_left) {
      n_left -= 1;
      lp = advance_line(lp);
    }
  }
  return validate_line_point(lp);
}
template <typename T>
doc_line_point_<T> doc_<T>::advance_line(doc_line_point_<T> lp,
                                         size_t n) const {
  return validate_line_point({lp.line_no + n, 0});
}
template <typename T>
doc_line_point_<T> doc_<T>::rewind(doc_line_point_<T> lp, size_t n) const {
  lp = validate_line_point(lp);

  if (!n) {
    return lp;
  }

  if (lp >= end()) {
    n -= 1;
    lp = doc_line_point_<T>{
        lp.line_no, std::vector<doc_line_<T>>::at(lp.line_no).last_char()};
  }

  size_t n_left = n;

  while (n_left) {

    if (lp == begin()) {
      return begin();
    }

    size_t n_in_line =
        (lp.character == std::basic_string<T>::npos)
            ? (std::vector<doc_line_<T>>::at(lp.line_no).last_char() + 1)
            : lp.character;

    if (n_left < n_in_line) {
      lp.character -= n_left;
      break;
    }
    n_left -= n_in_line;
    lp.character = 0;
    if (n_left) {
      n_left -= 1;
      lp = rewind_line(lp);
    }
  }

  return validate_line_point(lp);
}
template <typename T>
doc_line_point_<T> doc_<T>::rewind_line(doc_line_point_<T> lp, size_t n) const {
  if (lp.line_no < n) {
    return begin();
  }
  return validate_line_point({lp.line_no - n, std::basic_string<T>::npos});
}

template <typename T> bool doc_<T>::line_is_empty(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  if (lp == end()) {
    false;
  }
  return std::vector<doc_line_<T>>::at(lp.line_no).is_empty();
}

template <typename T>
size_t doc_<T>::get_filename_id(std::basic_string<T> const &filename) {
  if (!filename.size()) {
    return std::numeric_limits<size_t>::max();
  }
  for (size_t it = 0; it < filenames.size(); ++it) {
    if (filename == filenames[it]) {
      return it;
    }
  }
  filenames.push_back(filename);
  return (filenames.size() - 1);
}

template <typename T> T doc_<T>::get_char(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  if (is_end(lp)) {
    return EOF;
  }
  return std::vector<doc_line_<T>>::at(lp.line_no).get_char(lp.character);
}

template <typename T> void doc_<T>::tidy_filenames() {
  std::map<size_t, size_t> fnusage;

  for (size_t fn_it = 0; fn_it < filenames.size(); ++fn_it) {
    fnusage[fn_it] = 0;
  }

  for (size_t ln_it = 0; ln_it < std::vector<doc_line_<T>>::size(); ++ln_it) {
    doc_line_<T> const &line = std::vector<doc_line_<T>>::at(ln_it);
    if (fnusage.find(line.fileID) == fnusage.end()) {
      continue;
    }
    fnusage.at(line.fileID)++;
  }

  std::map<size_t, size_t> fnshuffle = {
      {std::basic_string<T>::npos, std::basic_string<T>::npos}};
  std::vector<std::basic_string<T>> filenames_new;

  for (size_t fn_it = 0; fn_it < filenames.size(); ++fn_it) {
    if (fnusage.at(fn_it)) {
      fnshuffle[fn_it] = fnshuffle.size();
      filenames_new.push_back(filenames[fn_it]);
    }
  }

  for (size_t ln_it = 0; ln_it < std::vector<doc_line_<T>>::size(); ++ln_it) {
    doc_line_<T> &line = std::vector<doc_line_<T>>::at(ln_it);
    if (fnshuffle.find(line.fileID) == fnshuffle.end()) {
      line.fileID = get_filename_id();
    } else {
      line.fileID = fnshuffle.at(line.fileID);
    }
  }

  filenames = std::move(filenames_new);
}

template <typename T> doc_line_point_<T> doc_<T>::begin() const {
  return validate_line_point(doc_line_point_<T>::begin());
}
template <typename T> doc_line_point_<T> doc_<T>::end() const {
  return validate_line_point(
      std::vector<doc_line_<T>>::size()
          ? doc_line_point_<T>{std::vector<doc_line_<T>>::size() - 1,
                               std::string::npos}
          : doc_line_point_<T>::begin());
}
template <typename T>
bool doc_<T>::are_equivalent(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return (l == r);
}

template <typename T>
bool doc_<T>::same_line(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return (l.line_no == r.line_no);
}

template <typename T>
bool doc_<T>::is_earlier(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return l < r;
}
template <typename T>
bool doc_<T>::is_later(doc_line_point_<T> l, doc_line_point_<T> r) const {
  l = validate_line_point(l);
  r = validate_line_point(r);
  return !(l < r) && !(l == r);
}
template <typename T>
doc_line_point_<T> doc_<T>::get_earlier(doc_line_point_<T> l,
                                        doc_line_point_<T> r) const {
  if (is_earlier(l, r)) {
    return l;
  }
  return r;
}
template <typename T>
doc_line_point_<T> doc_<T>::get_later(doc_line_point_<T> l,
                                      doc_line_point_<T> r) const {
  if (is_later(l, r)) {
    return l;
  }
  return r;
}
template <typename T> bool doc_<T>::is_begin(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  return lp == begin();
}
template <typename T> bool doc_<T>::is_end(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  return lp == end();
}

template <typename T> size_t doc_<T>::n_lines() const {
  return std::vector<doc_line_<T>>::size();
}

template <typename T>
doc_line_point_<T> doc_<T>::find_first_of(std::basic_string<T> const &chars,
                                          doc_line_point_<T> start,
                                          doc_line_point_<T> end) const {
  // As EOL characters are implicit, have to treat them somewhat specially.
  bool isEOL = (chars.find('\n') != std::basic_string<T>::npos);
  bool isOnlyEOL = isEOL && (chars.size() == 1);

  doc_line_point_<T> search = validate_line_point(start);
  end = validate_line_point(end);

  if (!is_earlier(search, end)) {
    return doc_<T>::end();
  }

  while (is_earlier(search, end)) {
    size_t pos = std::vector<doc_line_<T>>::at(search.line_no)
                     .characters.find_first_of(chars, search.character);
    if (pos == std::basic_string<T>::npos) {
      if (isEOL) { // we are looking for the EOL.
        search.character = pos;
        break;
      }
      search = advance_line(search);
      continue;
    }
    search.character = pos;
    break;
  }

  if (!is_earlier(search, end)) {
    return doc_<T>::end();
  }

  if (isOnlyEOL && !search.is_EOL()) {
    return doc_<T>::end();
  }
  if (chars.find(get_char(search)) == std::basic_string<T>::npos) {
    return doc_<T>::end();
  }

  return search;
}
template <typename T>
doc_line_point_<T> doc_<T>::find_first_of(T c, doc_line_point_<T> start,
                                          doc_line_point_<T> end) const {
  std::basic_stringstream<T> ss("");
  ss << c;
  return find_first_of(ss.str(), start, end);
}

template <typename T>
doc_line_point_<T> doc_<T>::find_first_not_of(std::basic_string<T> const &chars,
                                              doc_line_point_<T> start,
                                              doc_line_point_<T> end) const {
  // As EOL characters are implicit, have to treat them somewhat specially.
  bool isEOL = (chars.find('\n') != std::basic_string<T>::npos);
  bool isOnlyEOL = isEOL && (chars.size() == 1);

  doc_line_point_<T> search = validate_line_point(start);
  end = validate_line_point(end);

  if (!is_earlier(search, end)) {
    return doc_<T>::end();
  }

  while (is_earlier(search, end)) {
    size_t pos = std::vector<doc_line_<T>>::at(search.line_no)
                     .characters.find_first_not_of(chars, search.character);
    if (isEOL && (pos == std::basic_string<T>::npos)) {
      search = advance_line(search);
      continue;
    }
    search.character = pos;
    break;
  }

  if (!is_earlier(search, end)) {
    return doc_<T>::end();
  }

  if (isOnlyEOL && search.is_EOL()) {
    return doc_<T>::end();
  }
  if (chars.find(get_char(search)) != std::basic_string<T>::npos) {
    return doc_<T>::end();
  }

  return search;
}
template <typename T>
doc_line_point_<T> doc_<T>::find_first_not_of(T c, doc_line_point_<T> start,
                                              doc_line_point_<T> end) const {
  std::basic_stringstream<T> ss("");
  ss << c;
  return find_first_not_of(ss.str(), start, end);
}
template <typename T>
doc_line_point_<T> doc_<T>::find_last_of(std::basic_string<T> const &chars,
                                         doc_line_point_<T> start,
                                         doc_line_point_<T> end) const {
  // As EOL characters are implicit, have to treat them somewhat specially.
  bool isEOL = (chars.find('\n') != std::basic_string<T>::npos);
  bool isOnlyEOL = isEOL && (chars.size() == 1);

  doc_line_point_<T> search = rewind(end);
  start = validate_line_point(start);

  if (is_earlier(search, start)) {
    return doc_<T>::end();
  }

  while (is_later(search, start)) {

    if (isEOL && is_earlier(search.get_EOL(),
                            end)) { // if this EOL is allowed and we are
                                    // looking for an EOL, return it.
      return validate_line_point(search.get_EOL());
    }

    size_t pos = std::vector<doc_line_<T>>::at(search.line_no)
                     .characters.find_last_of(chars, search.character);
    if (pos == std::basic_string<T>::npos) {
      search = rewind_line(search);
      continue;
    }
    search.character = pos;
    break;
  }

  if (is_earlier(search, start)) {
    return doc_<T>::end();
  }

  if (isOnlyEOL && !search.is_EOL()) {
    return doc_<T>::end();
  }
  if (chars.find(get_char(search)) == std::basic_string<T>::npos) {
    return doc_<T>::end();
  }

  return search;
}
template <typename T>
doc_line_point_<T> doc_<T>::find_last_of(T c, doc_line_point_<T> start,
                                         doc_line_point_<T> end) const {
  std::basic_stringstream<T> ss("");
  ss << c;
  return find_first_not_of(ss.str(), start, end);
}
template <typename T>
doc_line_point_<T> doc_<T>::find_last_not_of(std::basic_string<T> const &chars,
                                             doc_line_point_<T> start,
                                             doc_line_point_<T> end) const {
  // As EOL characters are implicit, have to treat them somewhat specially.
  bool isEOL = (chars.find('\n') != std::basic_string<T>::npos);
  bool isOnlyEOL = isEOL && (chars.size() == 1);

  doc_line_point_<T> search = rewind(end);
  start = validate_line_point(start);

  if (is_earlier(search, start)) {
    return doc_<T>::end();
  }

  while (is_later(search, start)) {

    if (!isEOL && is_earlier(search.get_EOL(),
                             end)) { // if this EOL is allowed and we are
                                     // looking for an EOL, return it.
      return validate_line_point(search.get_EOL());
    }

    size_t pos = std::vector<doc_line_<T>>::at(search.line_no)
                     .characters.find_last_not_of(chars, search.character);
    if (pos == std::basic_string<T>::npos) {
      search = rewind_line(search);
      continue;
    }
    search.character = pos;
    break;
  }

  if (is_earlier(search, start)) {
    return doc_<T>::end();
  }

  if (isOnlyEOL && search.is_EOL()) {
    return doc_<T>::end();
  }

  if (chars.find(get_char(search)) != std::basic_string<T>::npos) {
    return doc_<T>::end();
  }

  return search;
}
template <typename T>
doc_line_point_<T> doc_<T>::find_last_not_of(T c, doc_line_point_<T> start,
                                             doc_line_point_<T> end) const {
  std::basic_stringstream<T> ss("");
  ss << c;
  return find_first_not_of(ss.str(), start, end);
}

template <typename T>
doc_<T> doc_<T>::subdoc(doc_line_point_<T> start,
                        doc_line_point_<T> end) const {
  doc_line_point_<T> ptr = validate_line_point(start);
  end = validate_line_point(end);
  doc_line_point_<T> last_full_line = rewind_line(end);

  if (is_later(ptr, end) || are_equivalent(ptr, end)) {
    return doc_<T>();
  }

  doc_<T> new_doc;

  // file metadata
  new_doc.filenames = filenames;

  if (ptr.character) {
    if (ptr.line_no == end.line_no) {
      new_doc.std::vector<doc_line_<T>>::push_back(
          std::vector<doc_line_<T>>::at(ptr.line_no)
              .subline(ptr.character, (end.character - ptr.character)));
      new_doc.tidy_filenames();
      return new_doc;
    }
    new_doc.std::vector<doc_line_<T>>::push_back(
        std::vector<doc_line_<T>>::at(ptr.line_no).subline(ptr.character));
    ptr = advance_line(ptr);
  }

  while (is_earlier(ptr, last_full_line)) {
    new_doc.std::vector<doc_line_<T>>::push_back(
        std::vector<doc_line_<T>>::at(ptr.line_no));
    ptr = advance_line(ptr);
  }

  if (is_earlier(ptr, end) ||
      (are_equivalent(ptr, end) && ((ptr.character == 0) || is_end(ptr)))) {
    new_doc.std::vector<doc_line_<T>>::push_back(
        std::vector<doc_line_<T>>::at(ptr.line_no).subline(0, end.character));
  }

  new_doc.tidy_filenames();

  return new_doc;
}
template <typename T> doc_<T> doc_<T>::subdoc(doc_range_<T> rng) const {
  return subdoc(rng.begin, rng.end);
}

template <typename T>
std::basic_string<T> doc_<T>::substr(doc_line_point_<T> begin,
                                     doc_line_point_<T> end) const {
  doc_line_point_<T> ptr = validate_line_point(begin);
  end = validate_line_point(end);
  doc_line_point_<T> last_full_line = rewind_line(end);

  if (is_later(ptr, end) || are_equivalent(ptr, end)) {
    return "";
  }

  std::basic_stringstream<T> ss("");
  bool char_added = false;
  if (ptr.character) {
    if (ptr.line_no == end.line_no) {
      return std::vector<doc_line_<T>>::at(ptr.line_no)
          .subline(ptr.character, (end.character - ptr.character))
          .characters;
    }
    ss << std::vector<doc_line_<T>>::at(ptr.line_no)
              .subline(ptr.character)
              .characters;

    char_added = true;
    ptr = advance_line(ptr);
  }

  while (is_earlier(ptr, last_full_line)) {
    if (char_added) {
      ss << " ";
    }
    ss << std::vector<doc_line_<T>>::at(ptr.line_no).characters;
    char_added = true;
    ptr = advance_line(ptr);
  }

  if (is_earlier(ptr, end) ||
      (are_equivalent(ptr, end) && ((ptr.character == 0) || is_end(ptr)))) {
    if (char_added) {
      ss << " ";
    }
    ss << std::vector<doc_line_<T>>::at(ptr.line_no)
              .subline(0, end.character)
              .characters;
  }
  return ss.str();
}
template <typename T>
std::basic_string<T> doc_<T>::substr(doc_range_<T> rng) const {
  return substr(rng.begin, rng.end);
}

template <typename T>
std::basic_string<T> doc_<T>::get_line(doc_line_point_<T> lp,
                                       bool highlight) const {
  lp = validate_line_point(lp);
  if (lp.line_no == std::numeric_limits<size_t>::max()) {
    std::basic_stringstream<T> ss("");
    ss << EOF;
    return ss.str();
  }
  if (!highlight) {
    return std::vector<doc_line_<T>>::at(lp.line_no).characters;
  }
  std::stringstream ss("");
  std::basic_string<T> ln =
      std::vector<doc_line_<T>>::at(lp.line_no).characters;
  for (size_t i = 0; i < ln.size(); ++i) {
    if (i == lp.character) {
      ss << "\033[31;47m";
    }
    ss << ln[i];
    if (i == lp.character) {
      ss << "\033[0m";
    }
  }
  if (lp.is_EOL()) {
    ss << "\033[31;47m\\EOL\033[0m";
  }
  return ss.str();
}

template <typename T>
std::basic_string<T> doc_<T>::get_line(size_t ln_it) const {
  return get_line(doc_line_point_<T>{ln_it, 0});
}

template <typename T>
std::basic_string<T> doc_<T>::get_line_info(doc_line_point_<T> lp) const {
  lp = validate_line_point(lp);
  std::basic_stringstream<T> ss("");
  if (lp.line_no == std::numeric_limits<size_t>::max()) {
    ss << "EOF";
    return ss.str();
  }
  if (std::vector<doc_line_<T>>::at(lp.line_no).fileID >= filenames.size()) {
    ss << "UNKNOWN FILE";
    return ss.str();
  }
  ss << filenames.at(std::vector<doc_line_<T>>::at(lp.line_no).fileID) << ":"
     << std::vector<doc_line_<T>>::at(lp.line_no).file_line_no;
  return ss.str();
}
template <typename T>
std::basic_string<T> doc_<T>::get_line_info(size_t ln_it) const {
  return get_line_info(doc_line_point_<T>{ln_it, 0});
}

template <typename T> void doc_<T>::insert(doc_<T> const &docu, size_t line) {
  std::map<size_t, size_t> fnshuffle = {
      {std::basic_string<T>::npos, std::basic_string<T>::npos}};
  for (size_t fn_it = 0; fn_it < docu.filenames.size(); ++fn_it) {
    fnshuffle[fn_it] = get_filename_id(docu.filenames[fn_it]);
  }

  for (size_t ln_it = 0; ln_it < docu.size(); ++ln_it) {
    std::vector<doc_line_<T>>::insert(
        std::vector<doc_line_<T>>::begin() + (line++),
        doc_line_<T>{fnshuffle.at(docu[ln_it].fileID), docu[ln_it].file_line_no,
                     docu[ln_it].characters});
  }
}

template <typename T> void doc_<T>::insert(doc_<T> &&docu, size_t line) {
  std::map<size_t, size_t> fnshuffle = {
      {std::basic_string<T>::npos, std::basic_string<T>::npos}};
  for (size_t fn_it = 0; fn_it < docu.filenames.size(); ++fn_it) {
    fnshuffle[fn_it] = get_filename_id(docu.filenames[fn_it]);
  }

  for (size_t ln_it = 0; ln_it < docu.size(); ++ln_it) {
    doc_line_<T> &ln = docu.std::vector<doc_line_<T>>::at(ln_it);
    ln.fileID = fnshuffle.at(ln.fileID);

    std::vector<doc_line_<T>>::insert(
        std::vector<doc_line_<T>>::begin() + (line++), std::move(ln));
  }
}

template <typename T> void doc_<T>::remove_line(doc_line_point_<T> lp) {
  lp = validate_line_point(lp);
  if (lp.line_no == std::numeric_limits<size_t>::max()) {
    return;
  }
  std::vector<doc_line_<T>>::erase(std::vector<doc_line_<T>>::begin() +
                                   lp.line_no);
}
template <typename T> void doc_<T>::remove_line(size_t line_no) {
  remove_line(doc_line_point_<T>{line_no, 0});
}

template <typename T>
void doc_<T>::push_back(std::basic_string<T> const &line,
                        std::basic_string<T> const &filename, size_t line_no) {
  std::vector<doc_line_<T>>::push_back(
      doc_line_<T>{get_filename_id(filename), line_no, line});
}

template <typename T>
void doc_<T>::push_back(std::basic_string<T> &&line,
                        std::basic_string<T> const &filename, size_t line_no) {
  std::vector<doc_line_<T>>::push_back(
      doc_line_<T>{get_filename_id(filename), line_no, std::move(line)});
}

} // namespace linedoc

#endif
