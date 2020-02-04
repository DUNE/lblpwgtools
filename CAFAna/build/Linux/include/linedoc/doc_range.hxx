#ifndef LINEDOC_DOC_RANGE_HXX_SEEN
#define LINEDOC_DOC_RANGE_HXX_SEEN

#include "linedoc/doc_line_point.hxx"

#include <string>

namespace linedoc {

/// A range of characters within a document
template <typename T> struct doc_range_ {
  doc_line_point_<T> begin;
  doc_line_point_<T> end;
  static doc_range_<T> whole_doc() {
    return {doc_line_point_<T>::begin(), doc_line_point_<T>::end()};
  }
};

typedef doc_range_<char> doc_range;
typedef doc_range_<wchar_t> wdoc_range;

} // namespace linedoc

#endif
