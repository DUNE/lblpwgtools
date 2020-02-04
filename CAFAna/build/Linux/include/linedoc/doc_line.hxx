#ifndef LINEDOC_DOC_LINE_HXX_SEEN
#define LINEDOC_DOC_LINE_HXX_SEEN

#include <string>

namespace linedoc {

/// Document line type
template <typename T> struct doc_line_ {
  /// The UID of the on-disk file that this line was read from
  size_t fileID;
  /// The line number in the on-disk file that this line corresponds to
  size_t file_line_no;
  /// The character line read from the on-disk file
  std::basic_string<T> characters;

  bool is_empty() const { return !characters.size(); }
  bool is_EOL(size_t character) const { return character >= characters.size(); }
  size_t last_char() const {
    return is_empty() ? std::string::npos : (characters.size() - 1);
  }
  T get_char(size_t character) const {
    return is_EOL(character) ? '\n' : characters[character];
  }
  size_t size() const { return characters.size(); }
  doc_line_<T> subline(size_t i, size_t n = std::basic_string<T>::npos) const {
    if (is_EOL(i)) {
      return doc_line_<T>{fileID, file_line_no, ""};
    }
    return doc_line_<T>{fileID, file_line_no, characters.substr(i, n)};
  }
};

typedef doc_line_<char> doc_line;
typedef doc_line_<wchar_t> wdoc_line;

} // namespace linedoc

#endif
