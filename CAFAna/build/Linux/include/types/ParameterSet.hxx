#ifndef FHICLCPP_SIMPLE_TYPES_PARAMETER_SET_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_PARAMETER_SET_HXX_SEEN

#include "types/Atom.hxx"
#include "types/Base.hxx"
#include "types/exception.hxx"

#include "string_parsers/from_string.hxx"
#include "string_parsers/md5.hxx"
#include "string_parsers/to_string.hxx"
#include "string_parsers/traits.hxx"

#include <cstdint>
#include <iomanip>
#include <istream>
#include <memory>

namespace linedoc {
template <typename T> struct doc_range_;
typedef doc_range_<char> doc_range;
} // namespace linedoc

// These declarations must be here before the first instantation that would use
// str2T/T2Str in a given translation unit
namespace fhicl {
class ParameterSet;
namespace string_parsers {
template <>
inline fhicl::ParameterSet str2T<fhicl::ParameterSet>(std::string const &);
template <>
inline std::string T2Str<fhicl::ParameterSet>(fhicl::ParameterSet const &);
} // namespace string_parsers
} // namespace fhicl

namespace fhicl {
enum class fhicl_category;

// Forward declarations of functions found in utility.hxx
std::shared_ptr<Base> deep_copy_value(std::shared_ptr<Base> const original);
fhicl_category get_fhicl_category(std::shared_ptr<Base> const el);
std::string get_fhicl_category_string(std::shared_ptr<Base> const el);

typedef uint32_t ParameterSetID;
typedef std::string key_t;

class fhicl_doc;

class ParameterSet : public Base {

  friend ParameterSet parse_fhicl_document(fhicl_doc const &,
                                           ParameterSet const &,
                                           ParameterSet const &,
                                           linedoc::doc_range, key_t const &);

  template <typename T>
  friend std::shared_ptr<T>
  deep_copy_resolved_reference_value(key_t const &, ParameterSet const &,
                                     ParameterSet const &);

  std::map<std::string, std::shared_ptr<Base>> internal_rep;
  std::map<std::string, std::vector<std::string>> history;

  ParameterSetID idCache;

  void added_key(key_t const &key) {
    std::stringstream ss("");
    ss << "Added a " << std::quoted(get_fhicl_category_string(key))
       << " via ParameterSet interface";
    history[key].push_back(ss.str());
  }
  void overrode_key(key_t const &key,
                    size_t seq_index = std::numeric_limits<size_t>::max()) {
    std::stringstream ss("");
    if (seq_index == std::numeric_limits<size_t>::max()) {
      ss << "Overriden with a " << std::quoted(get_fhicl_category_string(key))
         << " via ParameterSet interface";
    } else {
      std::stringstream ss2("");
      ss2 << key << "[" << seq_index << "]";
      ss << "Overrode element " << seq_index << " with a "
         << std::quoted(get_fhicl_category_string(ss2.str()))
         << " via ParameterSet interface";
    }

    history[key].push_back(ss.str());
  }
  void added_key_for_extension(key_t const &key, key_t const &full_key) {
    std::stringstream ss("");
    ss << "Added a " << std::quoted(get_fhicl_category_string(key))
       << " via ParameterSet interface for extension to key: "
       << std::quoted(full_key);
    history[key].push_back(ss.str());
  }

  template <typename T>
  inline typename std::enable_if<std::is_same<Base, T>::value, void>::type
  put_into_internal_rep(key_t const &key, T const &value);
  template <typename T>
  inline typename std::enable_if<(!std::is_same<Base, T>::value) &&
                                     std::is_base_of<Base, T>::value,
                                 void>::type
  put_into_internal_rep(key_t const &key, T const &value);
  template <typename T>
  inline typename std::enable_if<(!std::is_base_of<Base, T>::value) &&
                                     (!std::is_base_of<Base, T>::value),
                                 void>::type
  put_into_internal_rep(key_t const &key, T const &value);

  template <typename T>
  inline void put_with_custom_history(key_t const &key, T const &value,
                                      std::string const &hist_entry);

  template <typename T>
  inline typename std::enable_if<std::is_base_of<Base, T>::value, void>::type
  put_with_custom_history(key_t const &key, std::shared_ptr<T> &&value_ptr,
                          std::string const &hist_entry);

  inline void from(std::string const &str);

  bool valid_key(key_t const &key) const {
    char front = key.front();
    if ((!std::isalpha(front)) && (front != '_')) {
      return false;
    }
    if (key.back() == '.') {
      return false;
    }
    if (key.find_first_of(" \":\'@(){}") != std::string::npos) {
      return false;
    }
    size_t first_open_bracket = key.find_first_of("[");
    size_t last_matching_bracket = 0;
    while (first_open_bracket != std::string::npos) {
      size_t matching_bracket = string_parsers::find_matching_bracket(
          key, '[', ']', first_open_bracket);
      if (matching_bracket > last_matching_bracket) {
        last_matching_bracket = matching_bracket;
      }
      if (matching_bracket == std::string::npos) {
        return false;
      }
      first_open_bracket = key.find_first_of("[", matching_bracket);
    }
    size_t last_close_bracket = key.find_last_of("]");
    if ((last_close_bracket != std::string::npos) &&
        (last_close_bracket > last_matching_bracket)) {
      return false;
    }
    return true;
  }

  struct key_index_pair_t {
    key_t key;
    size_t index;
    bool has_index() { return index != std::numeric_limits<size_t>::max(); }
  };

  key_index_pair_t get_key_index(key_t key) const {
    size_t first_open_bracket = key.find_first_of("[");
    size_t index = std::numeric_limits<size_t>::max();
    if (first_open_bracket != std::string::npos) {
      size_t matching_bracket = string_parsers::find_matching_bracket(
          key, '[', ']', first_open_bracket);
      index = string_parsers::str2T<size_t>(key.substr(
          first_open_bracket + 1, first_open_bracket + 1 - matching_bracket));
    }
    return {key.substr(0, first_open_bracket), index};
  }

  inline std::shared_ptr<Base> &get_value(key_t const &key,
                                          bool allow_extend = false,
                                          bool allow_override = false);
  inline std::shared_ptr<Base> const &get_value(key_t const &key) const;
  inline std::shared_ptr<Base> &
  get_value_recursive(key_t const &key, bool allow_extend = false,
                      bool allow_override = false);
  inline std::shared_ptr<Base> const &
  get_value_recursive(key_t const &key) const;

  bool check_key(key_t const &key, bool throw_on_not_exist = false) const {
    if (!key.size()) {
      throw null_key();
    }
    if (!valid_key(key)) {
      throw invalid_key() << "[ERROR]: Invalid key " << std::quoted(key);
    }
    if (!get_value_recursive(key)) {
      if (throw_on_not_exist) {
        throw nonexistant_key() << "[ERROR]: Key " << std::quoted(key)
                                << " does not exist in parameter set.";
      }
      return false;
    }
    return true;
  }

  std::string get_fhicl_category_string(key_t const &key) const {
    check_key(key, true);
    return fhicl::get_fhicl_category_string(get_value_recursive(key));
  }

public:
  ParameterSet() : internal_rep(), history(), idCache(0) {}
  ParameterSet(std::string const &str) : internal_rep(), idCache(0) {
    from(str);
  }
  ParameterSet(ParameterSet &&other) { (*this) = std::move(other); }
  ParameterSet(ParameterSet const &other) : ParameterSet() { splice(other); }

  ParameterSet &operator=(ParameterSet const &other) {
    // Get rid of any internal state
    internal_rep.clear();
    history.clear();

    splice(other);
    return *this;
  }

  ParameterSet &operator=(ParameterSet &&other) {
    internal_rep = std::move(other.internal_rep);
    history = std::move(other.history);
    idCache = other.idCache;
    return *this;
  }

  bool is_empty() const { return !internal_rep.size(); }

  ParameterSetID id() {
    if (idCache) {
      return idCache;
    }
    idCache = dynamic_cast<ParameterSet const *>(this)->id();
    return idCache;
  }

  ParameterSetID id() const {
    if (idCache) {
      return idCache;
    }
    // if we can't use the cache, we also cannot update it as this is a const
    // method.
    std::string md = md5(to_string());
    ParameterSetID ID = 1;
    while (md.size()) {
      ID *= string_parsers::str2T<ParameterSetID>(std::string("0x") +
                                                  md.substr(0, 4));
      md = md.substr(4);
    }
    return ID;
  }

  std::string to_string() const {
    std::stringstream ss("");
    for (auto ip_it = internal_rep.begin(); ip_it != internal_rep.end();
         ++ip_it) {
      std::shared_ptr<ParameterSet> ps =
          std::dynamic_pointer_cast<ParameterSet>(ip_it->second);
      if (ps) {
        ss << ip_it->first << ": { " << ip_it->second->to_string() << " } ";
      } else {
        ss << ip_it->first << ": " << ip_it->second->to_string() << " ";
      }
    }
    std::string str_rep = ss.str();
    return str_rep.substr(0, str_rep.size() - 1);
  }
  std::string to_compact_string() const {
    std::stringstream ss("");
    for (auto ip_it = internal_rep.cbegin(); ip_it != internal_rep.cend();
         ++ip_it) {
      std::shared_ptr<ParameterSet const> ps =
          std::dynamic_pointer_cast<ParameterSet const>(ip_it->second);
      if (ps) {
        ss << ip_it->first << ": @id::" << ps->id() << " ";
      } else {
        ss << ip_it->first << ": " << ip_it->second->to_compact_string() << " ";
      }
    }
    std::string str_rep = ss.str();
    return str_rep.substr(0, str_rep.size() - 1);
  }
  std::string to_indented_string(size_t indent_level = 0) const {
    std::stringstream ss("");
    size_t nprinted = 0;
    for (auto ip_it = internal_rep.cbegin(); ip_it != internal_rep.cend();
         ++ip_it) {
      for (size_t i_it = 0; i_it < indent_level; ++i_it) {
        ss << " ";
      }
      if (is_key_to_atom(ip_it->first)) {
        ss << ip_it->first << ": " << ip_it->second->to_indented_string(0)
           << ((nprinted + 1 == internal_rep.size()) ? "" : "\n");
      } else if (is_key_to_sequence(ip_it->first)) {
        ss << ip_it->first << ": [" << std::endl
           << ip_it->second->to_indented_string(indent_level +
                                                // ip_it->first.size() + 4)
                                                4)
           << std::endl;
        // for (size_t i_it = 0; i_it < (indent_level + ip_it->first.size() +
        // 2);
        // for (size_t i_it = 0; i_it < (indent_level + 2); ++i_it) {
        for (size_t i_it = 0; i_it < indent_level; ++i_it) {
          ss << " ";
        }
        ss << "]" << ((nprinted + 1 == internal_rep.size()) ? "" : "\n");
      } else {
        ss << ip_it->first << ": {" << std::endl
           << ip_it->second->to_indented_string(indent_level +
                                                // ip_it->first.size() + 4)
                                                4)
           << std::endl;
        // for (size_t i_it = 0; i_it < (indent_level + ip_it->first.size() +
        // 2);
        // for (size_t i_it = 0; i_it < (indent_level + 2); ++i_it) {
        for (size_t i_it = 0; i_it < indent_level; ++i_it) {
          ss << " ";
        }
        ss << "}" << ((nprinted + 1 == internal_rep.size()) ? "" : "\n");
      }
      nprinted++;
    }
    return ss.str();
  }
  std::string to_indented_string_with_src_info(size_t indent_level = 0) const {
    std::stringstream ss("");
    for (auto ip_it = internal_rep.cbegin(); ip_it != internal_rep.cend();
         ++ip_it) {
      for (size_t i_it = 0; i_it < indent_level; ++i_it) {
        ss << " ";
      }
      if (is_key_to_atom(ip_it->first)) {
        ss << ip_it->first << ": "
           << ip_it->second->to_indented_string_with_src_info(0) << std::endl;
      } else if (is_key_to_sequence(ip_it->first)) {
        ss << ip_it->first << ": "
           << ip_it->second->to_indented_string_with_src_info(
                  indent_level + ip_it->first.size() + 2);
      } else {
        ss << ip_it->first << ": {" << std::endl
           << ip_it->second->to_indented_string_with_src_info(
                  indent_level + ip_it->first.size() + 4)
           << std::endl;
        for (size_t i_it = 0; i_it < (indent_level + ip_it->first.size() + 2);
             ++i_it) {
          ss << " ";
        }
        ss << "}";
      }
      ss << "-- <" << get_src_info(ip_it->first) << ">" << std::endl;
    }
    return ss.str();
  }
  std::vector<key_t> get_names() const {
    std::vector<key_t> names;
    for (auto ip_it = internal_rep.cbegin(); ip_it != internal_rep.cend();
         ++ip_it) {
      names.push_back(ip_it->first);
    }
    return names;
  }
  std::vector<key_t> get_pset_names() const {
    std::vector<key_t> names;
    for (auto ip_it = internal_rep.cbegin(); ip_it != internal_rep.cend();
         ++ip_it) {
      std::shared_ptr<ParameterSet const> ps =
          std::dynamic_pointer_cast<ParameterSet const>(ip_it->second);
      if (ps) {
        names.push_back(ip_it->first);
      }
    }
    return names;
  }
  std::string get_src_info(key_t const &key) const {
    if (history.find(key) == history.end()) {
      return "";
    }
    std::stringstream ss("");
    for (size_t h_it = 0; h_it < history.at(key).size(); ++h_it) {
      ss << history.at(key)[h_it]
         << ((h_it + 1 == history.at(key).size()) ? "" : ", ");
    }
    return ss.str();
  }
  std::string history_to_string() const {
    std::stringstream ss("");
    for (auto &kvpair : history) {
      ss << kvpair.first << ": ";
      for (size_t h_it = 0; h_it < history.at(kvpair.first).size(); ++h_it) {
        ss << history.at(kvpair.first)[h_it]
           << ((h_it + 1 == history.at(kvpair.first).size()) ? "" : ", ");
      }
      ss << std::endl;
    }
    return ss.str();
  }

  bool has_key(key_t const &key) const { return check_key(key); }
  bool is_key_to_atom(key_t const &key) const {
    if (!check_key(key)) {
      return false;
    }
    std::shared_ptr<Atom const> atm =
        std::dynamic_pointer_cast<Atom const>(get_value_recursive(key));
    return bool(atm);
  }
  inline bool is_key_to_sequence(key_t const &key) const;
  bool is_key_to_table(key_t const &key) const {
    if (!check_key(key)) {
      return false;
    }
    std::shared_ptr<ParameterSet const> ps =
        std::dynamic_pointer_cast<ParameterSet const>(get_value_recursive(key));
    return bool(ps);
  }
  // get table
  template <typename T>
  typename std::enable_if<std::is_same<T, ParameterSet>::value, T>::type
  get(key_t const &key) const {
    check_key(key, true);
    if (!is_key_to_table(key)) {
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to retrieve key: " << std::quoted(key)
          << " as a fhicl table (fhicl::ParameterSet), but it corresponds to a "
          << std::quoted(get_fhicl_category_string(key));
    }
    std::shared_ptr<ParameterSet const> ps =
        std::dynamic_pointer_cast<ParameterSet const>(get_value_recursive(key));
    return *ps;
  }
  // get other
  template <typename T>
  typename std::enable_if<!std::is_same<T, ParameterSet>::value, T>::type
  get(key_t const &key) const {
    check_key(key, true);

    if (is_seq<T>::value && !is_key_to_sequence(key)) {
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to retrieve key: " << std::quoted(key)
          << " as a fhicl sequence ("
          << std::quoted(is_seq<T>::get_sequence_type())
          << "), but it corresponds to a "
          << std::quoted(get_fhicl_category_string(key));
    }
    if (!is_seq<T>::value && is_key_to_sequence(key)) {
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to retrieve key: " << std::quoted(key)
          << " as a fhicl atom, but it corresponds to a fhicl sequence";
    }

    if (is_seq<T>::value && is_key_to_table(key)) {
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to retrieve key: " << std::quoted(key)
          << " as a fhicl sequence ("
          << std::quoted(is_seq<T>::get_sequence_type())
          << "), but it corresponds to a fhicl table (ParameterSet)";
    }
    if (!is_seq<T>::value && is_key_to_table(key)) {
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to retrieve key: " << std::quoted(key)
          << " as a fhicl atom, but it corresponds to a fhicl table "
             "(ParameterSet)";
    }

    return string_parsers::str2T<T>(get_value_recursive(key)->to_string());
  };

  template <typename T> T get(key_t const &key, T def) const {
    try {
      return get<T>(key);
    } catch (fhicl::string_parsers::fhicl_cpp_simple_except &e) { // parser fail
      return def;
    } catch (fhicl::fhicl_cpp_simple_except &e) { // type fail
      return def;
    } catch (std::exception &e) {
      throw bizare_error()
          << "[ERROR]: Caught unexpected exception in ParameterSet::get: "
          << std::quoted(e.what());
    }
  };

  template <typename T> bool get_if_present(key_t const &key, T &rtn) const {
    if (!check_key(key)) {
      return false;
    }
    try {
      rtn = get<T>(key);
    } catch (fhicl::string_parsers::fhicl_cpp_simple_except &e) { // parser fail
      return false;
    } catch (fhicl::fhicl_cpp_simple_except &e) { // type fail
      return false;
    } catch (std::exception &e) {
      throw bizare_error()
          << "[ERROR]: Caught unexpected exception in ParameterSet::get: "
          << std::quoted(e.what());
    }
    return true;
  };

  template <typename T> void put(key_t const &key, T const &value) {
    if (has_key(key)) {
      throw cant_insert() << "[ERROR]: Cannot put with key: "
                          << std::quoted(key) << " as that key already exists.";
    }
    put_or_replace(key, value);
  }

  // put nil
  void put(key_t const &key) {
    if (has_key(key)) {
      throw cant_insert() << "[ERROR]: Cannot put with key: "
                          << std::quoted(key) << " as that key already exists.";
    }
    put_or_replace(key, "@nil");
  }

  template <typename T> void put_or_replace(key_t const &key, T const &value) {
    bool had_key = has_key(key);
    put_into_internal_rep(key, value);
    if (had_key) {
      added_key(key);
    } else {
      overrode_key(key);
    }
  }

  template <typename T>
  typename std::enable_if<std::is_same<T, ParameterSet>::value, void>::type
  put_or_replace_compatible(key_t const &key, T const &value) {
    if (has_key(key) && !is_key_to_table(key)) {
      throw cant_insert()
          << "[ERROR]: Cannot put fhicl table (ParameterSet) with key: "
          << std::quoted(key)
          << " as that key already exists and is of non-compatible type: "
          << std::quoted(get_fhicl_category_string(key));
    }
    put_or_replace(key, value);
  }

  template <typename T>
  typename std::enable_if<!std::is_same<T, ParameterSet>::value, void>::type
  put_or_replace_compatible(key_t const &key, T const &value) {
    if (has_key(key)) {
      if (is_seq<T>::value && !is_key_to_sequence(key)) {
        throw cant_insert()
            << "[ERROR]: Cannot put fhicl sequence ("
            << std::quoted(is_seq<T>::get_sequence_type())
            << ") with key: " << std::quoted(key)
            << " as that key already exists and is of non-compatible type: "
            << std::quoted(get_fhicl_category_string(key));
      }
      if (!is_seq<T>::value && is_key_to_sequence(key)) {
        throw cant_insert()
            << "[ERROR]: Cannot put fhicl atom with key: " << std::quoted(key)
            << " as that key already exists and is of non-compatible type: "
            << std::quoted(get_fhicl_category_string(key));
      }
    }
    put_or_replace(key, value);
  }

  void splice(ParameterSet const &other) {
    if (!other.internal_rep.size()) {
      return;
    }
    for (auto const &kv_pair : other.internal_rep) {
      bool had_key = has_key(kv_pair.first);
      internal_rep[kv_pair.first] =
          deep_copy_value(other.internal_rep.at(kv_pair.first));
      had_key ? overrode_key(kv_pair.first) : added_key(kv_pair.first);
      if (other.history.find(kv_pair.first) != other.history.end()) {
        for (auto const &h : other.history.at(kv_pair.first)) {
          history[kv_pair.first].push_back(h);
        }
      }
    }
    idCache = 0;
  }

  void splice(ParameterSet &&other) {
    if (!other.internal_rep.size()) {
      return;
    }
    for (auto const &kv_pair : other.internal_rep) {
      bool had_key = has_key(kv_pair.first);
      internal_rep[kv_pair.first] =
          std::move(other.internal_rep.at(kv_pair.first));
      had_key ? overrode_key(kv_pair.first) : added_key(kv_pair.first);
      if (other.history.find(kv_pair.first) != other.history.end()) {
        history[kv_pair.first].insert(
            history[kv_pair.first].end(),
            std::make_move_iterator(other.history.at(kv_pair.first).begin()),
            std::make_move_iterator(other.history.at(kv_pair.first).end()));
      }
    }
    idCache = 0;
  }

  bool erase(key_t const &key) {
    if (!has_key(key)) {
      return false;
    }
    internal_rep.erase(key);
    history.erase(key);
  }
};

} // namespace fhicl

namespace fhicl {
namespace string_parsers {
template <>
inline fhicl::ParameterSet str2T<fhicl::ParameterSet>(std::string const &str) {
  return fhicl::ParameterSet(str);
}
template <>
inline std::string T2Str<fhicl::ParameterSet>(fhicl::ParameterSet const &ps) {
  return std::string("{") + ps.to_string() + "}";
}
} // namespace string_parsers
} // namespace fhicl

#endif
