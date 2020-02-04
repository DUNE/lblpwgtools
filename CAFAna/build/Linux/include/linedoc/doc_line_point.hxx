#ifndef LINEDOC_DOC_LINE_POINT_HXX_SEEN
#define LINEDOC_DOC_LINE_POINT_HXX_SEEN

#include <limits>
#include <ostream>
#include <string>

namespace linedoc {

template <typename T> class doc_;

/// Document character pointer type
template <typename T> struct doc_line_point_ {

  friend class doc_<T>;

private:
  inline bool operator==(doc_line_point_<T> other) {
    return !((*this) < other) && !(other < (*this));
  }
  inline bool operator<(doc_line_point_<T> other) {
    return (line_no < other.line_no) ||
           ((line_no == other.line_no) && (character < other.character));
  }
  inline bool operator>=(doc_line_point_<T> other) {
    return !((*this) < other);
  }

public:
  /// Document line number
  ///
  ///\note `line_no == std::numeric_limits<size_t>::max()`` is always considered
  /// to be at the end of a document, but without a document for context it is
  /// not possible to tell if a given position is off the end
  size_t line_no;
  /// Character within line
  ///
  ///\note `line_no == std::basic_string<T>::npos`` is always considered to be
  /// at the end of a line, but without a document for context it is not
  /// possible to tell if a given position is off the end or not.
  size_t character;
  static doc_line_point_<T> end() {
    return doc_line_point_<T>{std::numeric_limits<size_t>::max(),
                              std::basic_string<T>::npos};
  }
  static doc_line_point_<T> begin() { return doc_line_point_<T>{0, 0}; }

  doc_line_point_<T> get_EOL() {
    return doc_line_point_<T>{line_no, std::basic_string<T>::npos};
  }

  bool is_EOL() { return (character == std::basic_string<T>::npos); }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, doc_line_point_<T> lp) {
  return os << "{" << lp.line_no << ", " << lp.character << " }";
}

typedef doc_line_point_<char> doc_line_point;
typedef doc_line_point_<wchar_t> wdoc_line_point;

} // namespace linedoc

#endif
