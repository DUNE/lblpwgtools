#ifndef FHICLCPP_SIMPLE_TYPES_COMPOSITE_TYPES_SHARED_IMPL_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_COMPOSITE_TYPES_SHARED_IMPL_HXX_SEEN

#include "types/Atom.hxx"
#include "types/Base.hxx"
#include "types/ParameterSet.hxx"
#include "types/Sequence.hxx"
#include "types/exception.hxx"
#include "types/traits.hxx"
#include "types/utility.hxx"

#include "string_parsers/exception.hxx"
#include "string_parsers/from_string.hxx"
#include "string_parsers/to_string.hxx"

#include <limits>
#include <memory>

namespace fhicl {

void Sequence::from(std::string const &str) {
  if (!str.size()) {
    return;
  }
  std::string tstr = str;
  string_parsers::trim(tstr);
  if (!string_parsers::well_bracket_wrapped(tstr, '[', ']')) {
    throw string_parsers::parser_fail()
        << "[ERROR]: Attempted to parse a non-array-like string ("
        << std::quoted(str)
        << ") as a Sequence -- expected to find list object wrapped in square "
           "brackets, e.g. "
        << std::quoted("[a, b, c, d]") << ".";

  } else {
    tstr = tstr.substr(1, tstr.size() - 2);
  }
  for (std::string const &outer_list_item :
       string_parsers::ParseToVect<std::string>(tstr, ",", true, true)) {
    if (string_parsers::is_table(outer_list_item)) {
      internal_rep.push_back(std::make_shared<ParameterSet>(outer_list_item));
    } else if (string_parsers::is_sequence(outer_list_item)) {
      internal_rep.push_back(std::make_shared<Sequence>(outer_list_item));
    } else {
      internal_rep.push_back(std::make_shared<Atom>(outer_list_item));
    }
  }
}

void ParameterSet::from(std::string const &str) {
  if (!str.size()) {
    return;
  }
  std::string tstr = str;
  string_parsers::trim(tstr);
  if (!string_parsers::well_bracket_wrapped(tstr, '{', '}')) {
    throw string_parsers::parser_fail()
        << "[ERROR]: Attempted to parse a non-map-like string ("
        << std::quoted(str)
        << ") as a ParameterSet -- expected to find dictionary object wrapped "
           "in braces, e.g. "
        << std::quoted("{a:b c:d}") << ".";

  } else {
    tstr = tstr.substr(1, tstr.size() - 2);
  }
  static const std::map<char, char> extra_brackets{
      {'[', ']'}, {'{', '}'}, {'(', ')'}, {'\"', '\"'}, {'\'', '\''}};
  std::vector<std::string> k_v_list = string_parsers::ParseToVect<std::string>(
      tstr, std::vector<std::string>{":", " "}, false, true, extra_brackets);
  if (k_v_list.size() % 2) {
    throw parser_fail()
        << "[ERROR]: Attempted to parse a map-like string but expected an "
           "even "
           "number of values to be parsed as "
        << std::quoted("key: value") << " pairs: " << std::quoted(tstr);
  }

  for (size_t i = 0; i < k_v_list.size(); i += 2) {

    std::string const &k = k_v_list[i];
    std::string const &v = k_v_list[i + 1];

    if (string_parsers::is_table(v)) {
      internal_rep.insert({k, std::make_shared<ParameterSet>(v)});
    } else if (string_parsers::is_sequence(v)) {
      internal_rep.insert({k, std::make_shared<Sequence>(v)});
    } else {
      internal_rep.insert({k, std::make_shared<Atom>(v)});
    }
  }
}

bool ParameterSet::is_key_to_sequence(key_t const &key) const {
  if (!check_key(key)) {
    return false;
  }
  std::shared_ptr<Sequence const> seq =
      std::dynamic_pointer_cast<Sequence const>(get_value_recursive(key));
  return bool(seq);
}

template <typename T>
typename std::enable_if<std::is_same<Base, T>::value, void>::type
ParameterSet::put_into_internal_rep(key_t const &key, T const &value) {
  try {
    ParameterSet const &ps = dynamic_cast<ParameterSet const &>(value);
    get_value_recursive(key, true, true) = std::make_shared<ParameterSet>(ps);
    idCache = 0;
    return;
  } catch (const std::bad_cast) {
  }
  try {
    Sequence const &seq = dynamic_cast<Sequence const &>(value);
    get_value_recursive(key, true, true) = std::make_shared<Sequence>(seq);
    idCache = 0;
    return;
  } catch (const std::bad_cast) {
  }
  try {
    Atom const &atm = dynamic_cast<Atom const &>(value);
    get_value_recursive(key, true, true) = std::make_shared<Atom>(atm);
    idCache = 0;
    return;
  } catch (const std::bad_cast) {
  }
  throw bizare_error() << "Failed to downcast internal representation to a "
                          "fhicl type for interal representation = "
                       << value.to_string();
}
template <typename T>
typename std::enable_if<(!std::is_same<Base, T>::value) &&
                            std::is_base_of<Base, T>::value,
                        void>::type
ParameterSet::put_into_internal_rep(key_t const &key, T const &value) {
  get_value_recursive(key, true, true) =
      std::make_shared<typename fhicl_type<T>::type>(value);
  idCache = 0;
}
template <typename T>
typename std::enable_if<(!std::is_base_of<Base, T>::value) &&
                            (!std::is_base_of<Base, T>::value),
                        void>::type
ParameterSet::put_into_internal_rep(key_t const &key, T const &value) {
  get_value_recursive(key, true, true) =
      std::make_shared<typename fhicl_type<T>::type>(
          string_parsers::T2Str<T>(value));
  idCache = 0;
}

template <typename T>
void ParameterSet::put_with_custom_history(key_t const &key, T const &value,
                                           std::string const &hist_entry) {
  std::stringstream ss("");
  if (has_key(key)) {
    ss << "Added a " << std::quoted(fhicl_type<T>::category_string())
       << " from " << std::quoted(hist_entry);
  } else {
    ss << "Overriden with a " << std::quoted(fhicl_type<T>::category_string())
       << " from " << std::quoted(hist_entry);
  }

  put_into_internal_rep(key, value);
  history[key].push_back(ss.str());
}

template <typename T>
typename std::enable_if<std::is_base_of<Base, T>::value, void>::type
ParameterSet::put_with_custom_history(key_t const &key,
                                      std::shared_ptr<T> &&value_ptr,
                                      std::string const &hist_entry) {
  std::stringstream ss("");
  if (has_key(key)) {
    ss << "Added a " << std::quoted(fhicl_type<T>::category_string())
       << " from " << std::quoted(hist_entry);
  } else {
    ss << "Overriden with a " << std::quoted(fhicl_type<T>::category_string())
       << " from " << std::quoted(hist_entry);
  }

  get_value_recursive(key, true, true) = std::move(value_ptr);
  idCache = 0;
  history[key].push_back(ss.str());
}

// #define DEBUG_GET_VALUE

#ifdef DEBUG_GET_VALUE
std::string indent = "";
#endif

std::shared_ptr<Base> &ParameterSet::get_value(key_t const &key,
                                               bool allow_extend,
                                               bool allow_override) {

#ifdef DEBUG_GET_VALUE
  std::cout << indent << "[GV]: Getting value of key: " << std::quoted(key)
            << std::endl;
#endif

  size_t first_period = key.find_first_of(".");
  if (first_period != std::string::npos) { // No recusion allowed
    throw bizare_error() << "[ERROR]: ParameterSet::get_value passed key "
                         << std::quoted(key)
                         << " but is not able to resolve FQKeys.";
  }

  auto ki_pair = get_key_index(key);
  auto kvp_it = internal_rep.find(ki_pair.key);
  if (kvp_it == internal_rep.end()) {
    if (allow_extend) {
      if (ki_pair.has_index()) { // if key is sequence-like
        std::shared_ptr<Sequence> seq = std::make_shared<Sequence>();
        internal_rep[ki_pair.key] = seq;
#ifdef DEBUG_GET_VALUE
        std::string oi = indent;
        indent = "+";
#endif
        added_key_for_extension(ki_pair.key, key);
#ifdef DEBUG_GET_VALUE
        indent = oi;
#endif
        return seq->get_or_extend_get_value(ki_pair.index);
      } else {
        internal_rep[ki_pair.key] = std::make_shared<Atom>();
#ifdef DEBUG_GET_VALUE
        std::string oi = indent;
        indent = "+";
#endif
        added_key_for_extension(ki_pair.key, key);
#ifdef DEBUG_GET_VALUE
        indent = oi;
#endif
        return internal_rep[ki_pair.key];
      }
    } else {
      return Base::empty();
    }
  } else {                     // key already exists
    if (ki_pair.has_index()) { // if key is sequence-like
#ifdef DEBUG_GET_VALUE
      std::string oi = indent;
      indent = "+";
#endif
      if (!is_key_to_sequence(
              ki_pair.key)) {  // is the existing value sequence-like?
        if (!allow_override) { // if not, and we can't do anything about it,
                               // throw
          throw wrong_fhicl_category()
              << "[ERROR]: Attempted to access index of a sequence with key: "
              << std::quoted(key) << ", however the value is of fhicl type "
              << std::quoted(get_fhicl_category_string(key));
        }
        // if we can, flatten whatever used to live there with an empty sequence
        internal_rep.at(ki_pair.key) = std::make_shared<Sequence>();
        overrode_key(ki_pair.key, ki_pair.index);
      }
#ifdef DEBUG_GET_VALUE
      indent = oi;
#endif
      // get the sequence
      std::shared_ptr<Sequence> seq =
          std::dynamic_pointer_cast<Sequence>(internal_rep.at(ki_pair.key));
      // return the relevant index, request extension if allowed
      return allow_extend ? seq->get_or_extend_get_value(ki_pair.index)
                          : seq->get(ki_pair.index);
    } else {
      return kvp_it->second;
    }
  }
}

std::shared_ptr<Base> const &ParameterSet::get_value(key_t const &key) const {

#ifdef DEBUG_GET_VALUE
  std::cout << indent << "[GV cst]: Getting value of key: " << std::quoted(key)
            << std::endl;
#endif

  size_t first_period = key.find_first_of(".");
  if (first_period != std::string::npos) { // No recusion allowed
    throw bizare_error() << "[ERROR]: ParameterSet::get_value passed key "
                         << std::quoted(key)
                         << " but is not able to resolve FQKeys.";
  }

  auto ki_pair = get_key_index(key);
  auto kvp_it = internal_rep.find(ki_pair.key);
  if (kvp_it ==
      internal_rep.end()) { // if the key doesn't exist, we cannot make it
    return Base::empty();
  }

  // key already exists

  if (ki_pair.has_index()) { // if key is sequence-like
#ifdef DEBUG_GET_VALUE
    std::string oi = indent;
    indent = "+";
#endif
    if (!is_key_to_sequence(ki_pair.key)) { // is the existing value
                                            // sequence-like?
      // if not, we can't do anything about it, throw
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to access index of a sequence with key: "
          << std::quoted(key) << ", however the value is of fhicl type "
          << std::quoted(get_fhicl_category_string(key));
    }
#ifdef DEBUG_GET_VALUE
    indent = oi;
#endif
    // if it is, get the sequence
    std::shared_ptr<Sequence const> const seq =
        std::dynamic_pointer_cast<Sequence const>(internal_rep.at(ki_pair.key));
    // return the relevant index or Base::empty if it doesn't exist.
    return seq->get(ki_pair.index);
  } else {
    return kvp_it->second;
  }
}

std::shared_ptr<Base> &ParameterSet::get_value_recursive(key_t const &key,
                                                         bool allow_extend,
                                                         bool allow_override) {
  if (!key.size()) {
    throw null_key();
  }
  if (!valid_key(key)) {
    throw invalid_key() << "[ERROR]: Invalid key " << std::quoted(key);
  }

#ifdef DEBUG_GET_VALUE
  std::cout << indent << "[GVR]: Getting value of key: " << std::quoted(key)
            << std::endl;
#endif

  size_t first_period = key.find_first_of(".");
  if (first_period == std::string::npos) { // No more recusion
    return get_value(key, allow_extend, allow_override);
  }
  // we must go deepr
  std::string local_key = key.substr(0, first_period);
  std::shared_ptr<Base> &local_value =
      get_value(local_key, allow_extend, allow_override);

  if (!local_value) { // We didn't get a value, it doesn't exist and we aren't
                      // allowed to create it
    return local_value;
  }

  std::shared_ptr<ParameterSet> child_table =
      std::dynamic_pointer_cast<ParameterSet>(local_value);

  if (!child_table) {      // if the value is not of ParameterSet type
    if (!allow_override) { // and we aren't allowed to override, throw
      throw wrong_fhicl_category()
          << "[ERROR]: Attempted to recurse into fhicl table: "
          << std::quoted(local_key)
          << " for resolution of: " << std::quoted(key)
          << ", however the value is of fhicl type "
          << std::quoted(get_fhicl_category_string(key));
    }
    // Flatten with a table.
    local_value = std::make_shared<ParameterSet>();
    child_table = std::dynamic_pointer_cast<ParameterSet>(local_value);
  }

#ifdef DEBUG_GET_VALUE
  indent += "  ";
  std::shared_ptr<Base> &rtn = child_table->get_value_recursive(
      key.substr(first_period + 1), allow_extend, allow_override);
  indent = indent.substr(2);
  return rtn;
#else
  return child_table->get_value_recursive(key.substr(first_period + 1),
                                          allow_extend, allow_override);
#endif
}

std::shared_ptr<Base> const &
ParameterSet::get_value_recursive(key_t const &key) const {
  if (!key.size()) {
    throw null_key();
  }
  if (!valid_key(key)) {
    throw invalid_key() << "[ERROR]: Invalid key " << std::quoted(key);
  }

#ifdef DEBUG_GET_VALUE
  std::cout << indent << "[GVR cst]: Getting value of key: " << std::quoted(key)
            << std::endl;
#endif

  size_t first_period = key.find_first_of(".");
  if (first_period == std::string::npos) { // No more recusion
    return get_value(key);
  }
  // we must go deepr
  std::string local_key = key.substr(0, first_period);
  std::shared_ptr<Base> const &local_value = get_value(local_key);

  if (!local_value) { // We didn't get a value, it doesn't exist and we aren't
                      // allowed to create it
    return local_value;
  }

  std::shared_ptr<ParameterSet const> const child_table =
      std::dynamic_pointer_cast<ParameterSet const>(local_value);

  if (!child_table) { // if the value is not of ParameterSet type, we aren't
                      // allowed to override, throw
    throw wrong_fhicl_category()
        << "[ERROR]: Attempted to recurse into fhicl table: "
        << std::quoted(local_key) << " for resolution of: " << std::quoted(key)
        << ", however the value is of fhicl type "
        << std::quoted(get_fhicl_category_string(key));
  }

#ifdef DEBUG_GET_VALUE
  indent += "  ";
  std::shared_ptr<Base> const &rtn =
      child_table->get_value_recursive(key.substr(first_period + 1));
  indent = indent.substr(2);
  return rtn;
#else
  return child_table->get_value_recursive(key.substr(first_period + 1));
#endif
}

} // namespace fhicl

#endif
