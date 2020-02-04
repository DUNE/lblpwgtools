#ifndef FHICLCPP_SIMPLE_TYPES_SEQUENCE_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_SEQUENCE_HXX_SEEN

#include "types/Base.hxx"

#include "string_parsers/from_string.hxx"
#include "string_parsers/traits.hxx"

#include <memory>

namespace fhicl {
class ParameterSet;
// forward declaration for functions found in utility.hxx
std::shared_ptr<Base> deep_copy_value(std::shared_ptr<Base> const original);

class Sequence : public Base {
  std::vector<std::shared_ptr<Base>> internal_rep;

  inline void from(std::string const &str);

public:
  std::shared_ptr<Base> &get_or_extend_get_value(size_t idx) {
    if (idx >= internal_rep.size()) {
      while (idx >= internal_rep.size()) {
        internal_rep.push_back(std::make_shared<Atom>());
      }
    }
    return internal_rep[idx];
  }
  std::shared_ptr<Base> &get(size_t idx) {
    if (idx >= internal_rep.size()) {
      return Base::empty();
    }
    return internal_rep[idx];
  }
  std::shared_ptr<Base> const &get(size_t idx) const {
    static std::shared_ptr<Base> const nullrtn(nullptr);
    if (idx >= internal_rep.size()) {
      return nullrtn;
    }
    return internal_rep[idx];
  }

  void put(std::shared_ptr<Base> const &obj) {
    internal_rep.push_back(deep_copy_value(obj));
  }

  void put(std::shared_ptr<Base> &&obj) {
    internal_rep.push_back(std::move(obj));
  }

  size_t size() const { return internal_rep.size(); }

  template <typename T> T at_as(size_t index) const {
    if (internal_rep.size() <= index) {
      throw;
    }
    return string_parsers::str2T<T>(internal_rep[index]->to_string());
  };
  template <typename T>
  typename std::enable_if<is_seq<T>::value, T>::type as() const {
    return string_parsers::str2T<T>(to_string());
  };
  Sequence() : internal_rep() {}
  Sequence(std::string const &str) : internal_rep() { from(str); }
  Sequence(Sequence &&other) : internal_rep(std::move(other.internal_rep)) {}
  Sequence(Sequence const &other) : Sequence() { splice(other); }

  std::string to_string() const {
    std::stringstream ss("");
    ss << "[";
    for (size_t i = 0; i < internal_rep.size(); ++i) {
      std::shared_ptr<ParameterSet> ps =
          std::dynamic_pointer_cast<ParameterSet>(internal_rep[i]);
      if (ps) {
        ss << "{ " << internal_rep[i]->to_string() << "} "
           << ((i + 1 == internal_rep.size()) ? "" : ", ");
      } else {
        ss << internal_rep[i]->to_string()
           << ((i + 1 == internal_rep.size()) ? "" : ", ");
      }
    }
    ss << "]";
    return ss.str();
  }
  std::string to_compact_string() const {
    std::stringstream ss("");
    ss << "[";
    for (size_t i = 0; i < internal_rep.size(); ++i) {
      std::shared_ptr<ParameterSet> ps =
          std::dynamic_pointer_cast<ParameterSet>(internal_rep[i]);
      if (ps) {
        ss << "{ " << internal_rep[i]->to_compact_string() << "}"
           << ((i + 1 == internal_rep.size()) ? "" : ",");
      } else {
        ss << internal_rep[i]->to_compact_string()
           << ((i + 1 == internal_rep.size()) ? "" : ",");
      }
    }
    ss << "]";
    return ss.str();
  }
  std::string to_indented_string(size_t indent_level) const {
    std::stringstream ss("");
    for (size_t i = 0; i < internal_rep.size(); ++i) {
      for (size_t i_it = 0; i_it < indent_level; ++i_it) {
        ss << " ";
      }
      std::shared_ptr<ParameterSet> ps =
          std::dynamic_pointer_cast<ParameterSet>(internal_rep[i]);
      std::shared_ptr<Sequence> seq =
          std::dynamic_pointer_cast<Sequence>(internal_rep[i]);
      if (ps) {
        ss << "{ " << std::endl
           << internal_rep[i]->to_indented_string(indent_level + 2)
           << std::endl;
        for (size_t i_it = 0; i_it < indent_level; ++i_it) {
          ss << " ";
        }
        ss << "}" << ((i + 1 == internal_rep.size()) ? "" : ",\n");
      } else if (seq) {
        ss << "[ " << std::endl
           << internal_rep[i]->to_indented_string(indent_level + 2)
           << std::endl;
        for (size_t i_it = 0; i_it < indent_level; ++i_it) {
          ss << " ";
        }
        ss << "]" << ((i + 1 == internal_rep.size()) ? "" : ",\n");
      } else {
        ss << internal_rep[i]->to_indented_string(indent_level + 2)
           << ((i + 1 == internal_rep.size()) ? "" : ",\n");
      }
    }
    return ss.str();
  }
  std::string to_indented_string_with_src_info(size_t indent_level) const {
    std::stringstream ss("");
    ss << "[" << std::endl;
    for (size_t i = 0; i < internal_rep.size(); ++i) {
      for (size_t i_it = 0; i_it < indent_level + 2; ++i_it) {
        ss << " ";
      }
      std::shared_ptr<ParameterSet> ps =
          std::dynamic_pointer_cast<ParameterSet>(internal_rep[i]);
      if (ps) {
        ss << "{ " << std::endl
           << internal_rep[i]->to_indented_string_with_src_info(indent_level +
                                                                4)
           << std::endl;
        for (size_t i_it = 0; i_it < (indent_level + 2); ++i_it) {
          ss << " ";
        }
        ss << "}"
           << "-- <NOINFO>" << ((i + 1 == internal_rep.size()) ? "" : ",");
      } else {
        ss << internal_rep[i]->to_indented_string_with_src_info(indent_level +
                                                                2)
           << ((i + 1 == internal_rep.size()) ? "" : ",");
      }
      ss << std::endl;
    }
    for (size_t i_it = 0; i_it < indent_level; ++i_it) {
      ss << " ";
    }
    ss << "]";
    return ss.str();
  }

  void splice(Sequence const &other,
              size_t indx = std::numeric_limits<size_t>::max()) {
    std::vector<std::shared_ptr<Base>>::iterator insert_it = internal_rep.end();

    if (indx <= internal_rep.size()) {
      insert_it = internal_rep.begin();
      std::advance(insert_it, indx);
    }

    for (size_t it = 0; it < other.internal_rep.size(); ++it) {
      insert_it = internal_rep.insert(insert_it,
                                      deep_copy_value(other.internal_rep[it]));
      std::advance(insert_it, 1);
    }
  }

  void splice(Sequence &&other,
              size_t indx = std::numeric_limits<size_t>::max()) {
    std::vector<std::shared_ptr<Base>>::iterator insert_it = internal_rep.end();

    if (indx <= internal_rep.size()) {
      insert_it = internal_rep.begin();
      std::advance(insert_it, indx);
    }

    for (size_t it = 0; it < other.internal_rep.size(); ++it) {
      insert_it =
          internal_rep.insert(insert_it, std::move(other.internal_rep[it]));
      std::advance(insert_it, 1);
    }
  }
};
} // namespace fhicl

#endif
