#ifndef LINEDOC_DOC_UTILS_HXX_SEEN
#define LINEDOC_DOC_UTILS_HXX_SEEN

#include "linedoc/doc.hxx"

#include <fstream>
#include <memory>
#include <algorithm>
#include <cstdio>

namespace linedoc {
inline doc read_doc(std::string const &filename) {
  std::unique_ptr<std::ifstream> ifs =
      std::make_unique<std::ifstream>(filename);
  if (!ifs || !ifs->good()) {
    return doc();
  }
  std::string line;
  doc new_doc;
  size_t ctr = 1;
  while (std::getline(*ifs, line)) {
    new_doc.push_back(line, filename, ctr);
    ctr++;
  }
  return new_doc;
}

} // namespace linedoc

#endif
