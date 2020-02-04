#ifndef FHICLCPP_SIMPLE_FHICLCPP_RECURSIVE_BUILD_FHICL_H_SEEN
#define FHICLCPP_SIMPLE_FHICLCPP_RECURSIVE_BUILD_FHICL_H_SEEN

#include "fhiclcpp/ParameterSet.h"

#include "fhiclcpp/fhicl_doc.hxx"

#include <cstdio>

namespace fhicl {

inline ParameterSet parse_fhicl_document(fhicl_doc const &,
                                         ParameterSet const &,
                                         ParameterSet const &,
                                         linedoc::doc_range, key_t const &);

// #define FHICLCPP_SIMPLE_PARSERS_DEBUG

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
std::string indent = "";
#endif

template <typename T>
inline std::shared_ptr<T>
deep_copy_resolved_reference_value(key_t const &key,
                                   ParameterSet const &working_set,
                                   ParameterSet const &PROLOG) {

  std::shared_ptr<Base> base_val = working_set.get_value_recursive(key);
  std::shared_ptr<Base> PROLOG_val = PROLOG.get_value_recursive(key);

  if ((!base_val) && (!PROLOG_val)) {
    std::stringstream ss("");
    ss << std::endl
       << "\t PROLOG: { " << PROLOG.to_string() << "}" << std::endl
       << "\t working_set: { " << working_set.to_string() << "}" << std::endl;
    throw nonexistant_key()
        << "[ERROR]: Failed to resolve reference directive as key: "
        << std::quoted(key)
        << " cannot be found. N.B. Reference keys must be fully qualified. "
           "\nCurrent document:"
        << ss.str();
  }

  // Non-PROLOG takes precedence
  std::shared_ptr<T> value_for_ref =
      std::dynamic_pointer_cast<T>(working_set.get_value_recursive(key));
  if (value_for_ref) {
    return std::dynamic_pointer_cast<T>(deep_copy_value(value_for_ref));
  } else if (base_val) {
    throw wrong_fhicl_category()
        << "[ERROR]: Attempted to resolve reference to key: "
        << std::quoted(key)
        << " as fhicl category: " << fhicl_type<T>::category_string()
        << " but resolved key is of type: "
        << working_set.get_fhicl_category_string(key);
  }

  std::shared_ptr<T> PROLOG_value_for_ref =
      std::dynamic_pointer_cast<T>(PROLOG.get_value_recursive(key));
  if (PROLOG_value_for_ref) {
    return std::dynamic_pointer_cast<T>(deep_copy_value(PROLOG_value_for_ref));
  } else if (PROLOG_val) {
    throw wrong_fhicl_category()
        << "[ERROR]: Attempted to resolve reference to key: "
        << std::quoted(key)
        << " as fhicl category: " << fhicl_type<T>::category_string()
        << " but resolved key is of type: "
        << PROLOG.get_fhicl_category_string(key);
  }

  return nullptr;
}

inline std::shared_ptr<Base>
parse_object(fhicl_doc const &doc, linedoc::doc_range range,
             linedoc::doc_line_point &next_character,
             ParameterSet const &working_set, ParameterSet const &PROLOG,
             key_t const &current_key, bool build_sequence = false) {

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
  std::cout << indent << "[INFO]: Parsing next object between: " << range.begin
            << " = " << std::quoted(doc.get_line(range.begin, true)) << " and "
            << range.end << " = " << std::quoted(doc.get_line(range.end, true))
            << std::endl;
#endif

  linedoc::doc_line_point next_not_break =
      doc.find_first_not_of(" \n", range.begin, range.end);

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
  std::cout << indent << "[INFO]: Next not space found at: "
            << std::quoted(doc.get_line(next_not_break, true)) << std::endl;
#endif
  char next_not_break_char = doc.get_char(next_not_break);
  switch (next_not_break_char) {
  case '{': {
    // table member
    linedoc::doc_line_point matching_bracket =
        find_matching_bracket(doc, '{', '}', next_not_break);
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Found table KV: {"
              << std::quoted(current_key) << ": {"
              << std::quoted(
                     doc.substr(doc.advance(next_not_break), matching_bracket))
              << "} } at " << doc.get_line_info(next_not_break) << std::endl;
    indent += "  ";
#endif
    std::shared_ptr<ParameterSet> table =
        std::make_shared<ParameterSet>(parse_fhicl_document(
            doc, working_set, PROLOG,
            {doc.advance(next_not_break), matching_bracket}, current_key));

    next_character = doc.advance(matching_bracket);
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    indent = indent.substr(2);
#endif
    return table;
  }
  case '[': {
    // list member
    linedoc::doc_line_point seq_end =
        find_matching_bracket(doc, '[', ']', next_not_break);

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Found sequence object: "
              << std::quoted(doc.substr(next_not_break, doc.advance(seq_end)))
              << ". at " << std::quoted(doc.get_line(next_not_break, true))
              << " from " << doc.get_line_info(next_not_break) << std::endl;
#endif

    std::shared_ptr<Sequence> seq = std::make_shared<Sequence>();
    auto const &seq_element_str_reps =
        get_list_elements(doc, {doc.advance(next_not_break), seq_end}, true);

    for (size_t el_it = 0; el_it < seq_element_str_reps.size(); ++el_it) {
      linedoc::doc_range el_range = seq_element_str_reps[el_it];
      linedoc::doc_line_point last_parsed_char;
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << " -- Sequence element between "
                << std::quoted(doc.get_line(el_range.begin, true)) << " and "
                << std::quoted(doc.get_line(el_range.end, true)) << " = "
                << std::quoted(doc.substr(el_range)) << std::endl;
      indent += "  ";
#endif
      if (!doc.substr(el_range).size()) {
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
        std::cout << indent << " -- Skipping!" << std::endl;
        indent = indent.substr(2);
#endif
        continue;
      }
      std::shared_ptr<Base> el_obj =
          parse_object(doc, el_range, last_parsed_char, working_set, PROLOG,
                       current_key, true);
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      indent = indent.substr(2);
#endif
      std::string unused_chars = doc.substr(last_parsed_char, el_range.end);
      string_parsers::trim(unused_chars);
      if (unused_chars.size()) {
        throw unexpected_newline()
            << "[ERROR]: When parsing sequence, element #" << el_it
            << " started at " << el_range.begin << " on line "
            << std::quoted(doc.get_line(el_range.begin, true)) << " from "
            << std::quoted(doc.get_line_info(el_range.begin))
            << ". Did not use: " << std::quoted(unused_chars) << " on line "
            << std::quoted(doc.get_line(last_parsed_char, true)) << " from "
            << std::quoted(doc.get_line_info(last_parsed_char))
            << " was there a newline in the middle of an atom element?";
      }

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << "[INFO]: Sequence element[" << el_it
                << "]: " << el_obj->to_string() << " read from "
                << std::quoted(doc.get_line(el_range.begin, true)) << " to "
                << std::quoted(doc.get_line(last_parsed_char, true))
                << std::endl;
#endif
      // Handle the result of @sequence directives.
      std::shared_ptr<Sequence> child_seq =
          std::dynamic_pointer_cast<Sequence>(el_obj);
      if (child_seq) {
        std::string str_rep = doc.substr(el_range);
        string_parsers::trim(str_rep);
        if (str_rep.find("@sequence") == 0) { // is sequence directive, splice
          seq->splice(std::move(*child_seq));
        } else {
          seq->put(std::move(el_obj));
        }
      } else {
        seq->put(std::move(el_obj));
      }
    }
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Parsed sequence = " << seq->to_string()
              << std::endl;
#endif
    next_character = doc.advance(seq_end);
    return seq;
  }
  case '\'':
  case '\"': {
    if (next_not_break.line_no != range.begin.line_no) {
      throw unexpected_newline()
          << "[ERROR]: When searching for value to key defined on line: "
          << std::quoted(doc.get_line(range.begin, true)) << " at "
          << std::quoted(doc.get_line_info(range.begin))
          << " found a string value starting at "
          << std::quoted(doc.get_line_info(next_not_break))
          << ", only tables and sequences may start on a new line.";
    }
    linedoc::doc_line_point first_string_point = doc.advance(next_not_break);
    // string member
    linedoc::doc_line_point matching_quote = doc.find_first_of(
        next_not_break_char, first_string_point, next_not_break.get_EOL());

    if (doc.is_end(matching_quote)) {
      throw unexpected_newline()
          << "[ERROR]: Failed to find matching quote to: "
          << std::quoted(doc.get_line(next_not_break, true)) << " from "
          << std::quoted(doc.get_line_info(next_not_break))
          << ". N.B. quoted strings cannot span multiple lines.";
    }

    std::string value = doc.substr(first_string_point, matching_quote);
    next_character = doc.advance(matching_quote);
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Found KV: {" << std::quoted(current_key)
              << ":" << std::quoted(value) << "}. at "
              << doc.get_line_info(next_not_break) << std::endl;
#endif
    return std::make_shared<Atom>(value);
  }
  case '@': {
    if (next_not_break.line_no != range.begin.line_no) {
      throw unexpected_newline()
          << "[ERROR]: When searching for value to key defined on line: "
          << std::quoted(doc.get_line(range.begin, true)) << " at "
          << std::quoted(doc.get_line_info(range.begin))
          << " found a fhicl directive starting at "
          << std::quoted(doc.get_line_info(next_not_break))
          << ", only tables and sequences may start on a new line.";
    }

    linedoc::doc_range directive_range;
    directive_range.begin = doc.advance(next_not_break);
    directive_range.end = doc.find_first_of(" :", directive_range.begin,
                                            directive_range.begin.get_EOL());
    if (doc.is_end(directive_range.end)) {
      throw malformed_document()
          << "[ERROR]: Found incomplete fhicl directive beginning: "
          << std::quoted(doc.get_line(next_not_break, true))
          << ", expecting one of \"nil\", \"local\", \"table\", or "
             "\"sequence\". From document: "
          << std::quoted(doc.get_line_info(next_not_break));
    }

    std::string directive = doc.substr(directive_range);

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Found directive: " << std::quoted(directive)
              << " between "
              << std::quoted(doc.get_line(directive_range.begin, true))
              << " and " << std::quoted(doc.get_line(directive_range.end, true))
              << std::endl;
#endif

    if (directive == "nil") {
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << "[INFO]: Found nil directive at "
                << std::quoted(doc.get_line(next_not_break, true)) << std::endl;
#endif
      return std::make_shared<Atom>("@nil");
    } else {
      std::string dc =
          doc.substr(directive_range.end, doc.advance(directive_range.end, 2));
      if (dc != "::") {
        throw malformed_document()
            << "[ERROR]: Expected to find double colon separator between "
               "\"@"
            << directive << "\" and the key name argument on line: "
            << std::quoted(doc.get_line(next_character, true))
            << " at: " << doc.advance(next_character) << ", but instead found: "
            << std::quoted(doc.substr(directive_range.end,
                                      doc.advance(directive_range.end, 2)));
      }
      linedoc::doc_range directive_key_range;
      directive_key_range.begin = doc.advance(directive_range.end, 2);
      directive_key_range.end =
          doc.find_first_of((build_sequence ? " \n," : " \n"),
                            directive_key_range.begin, range.end);

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << "[INFO]: Directive key between: "
                << std::quoted(doc.get_line(directive_key_range.begin, true))
                << " at "
                << std::quoted(doc.get_line_info(directive_key_range.begin))
                << " and "
                << std::quoted(doc.get_line(directive_key_range.end, true))
                << " at "
                << std::quoted(doc.get_line_info(directive_key_range.end))
                << " = " << std::quoted(doc.substr(directive_key_range))
                << std::endl;
#endif

      if (doc.is_end(directive_key_range.end)) { // hit the EOL
        directive_key_range.end = range.end;
      }

      next_character = directive_key_range.end;

      key_t directive_key = doc.substr(directive_key_range);
      if (directive == "local") {
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
        std::cout << indent << "[INFO]: Found local directive: "
                  << std::quoted(directive_key) << " at "
                  << std::quoted(doc.get_line(directive_range.begin, true))
                  << std::endl;
#endif
        return deep_copy_resolved_reference_value<Base>(directive_key,
                                                        working_set, PROLOG);
      } else if (directive == "table") {
        throw malformed_document()
            << "[ERROR]: Found @table directive "
            << (build_sequence ? " in sequence " : " with key ")
            << " but it should only be used directly within a table body to "
               "splice in a referenced table. Found at "
            << std::quoted(doc.get_line(directive_range.begin, true))
            << " from "
            << std::quoted(doc.get_line_info(directive_range.begin));
      } else if (directive == "sequence") {
        if (!build_sequence) {
          throw malformed_document()
              << "[ERROR]: Found @sequence directive outside of a sequence. "
                 "It can only be used within a sequence definition to splice "
                 "in a reference sequence. Found at "
              << std::quoted(doc.get_line(directive_range.begin, true))
              << " from "
              << std::quoted(doc.get_line_info(directive_range.begin));
        }
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
        std::cout << indent << "[INFO]: Found sequence directive: "
                  << std::quoted(directive_key) << " at "
                  << doc.get_line_info(next_not_break) << std::endl;
#endif
        return deep_copy_resolved_reference_value<Sequence>(
            directive_key, working_set, PROLOG);
      } else {
        throw malformed_document()
            << "[ERROR]: Unknown fhicl directive: " << std::quoted(directive)
            << ", expecting one of \"nil\", \"local\", \"table\", or "
               "\"sequence\". Found at "
            << std::quoted(doc.get_line(directive_range.begin, true))
            << " from "
            << std::quoted(doc.get_line_info(directive_range.begin));
      }
    }
  }
  default: { // simple atom type
    if (next_not_break.line_no != range.begin.line_no) {
      throw unexpected_newline()
          << "[ERROR]: When searching for value to key defined on line: "
          << std::quoted(doc.get_line(range.begin, true)) << " at "
          << std::quoted(doc.get_line_info(range.begin))
          << " found an atom starting at "
          << std::quoted(doc.get_line_info(next_not_break))
          << ", only tables and sequences may start on a new line.";
    }
    linedoc::doc_line_point next_break =
        doc.find_first_of(" \n", next_not_break, range.end);
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Next break from: " << next_not_break
              << " at " << std::quoted(doc.get_line(next_not_break, true))
              << " found as " << next_break << " at "
              << std::quoted(doc.get_line(next_break, true))
              << "(Max: " << range.end << "). " << std::endl;
#endif
    if (doc.is_end(next_break)) { // If you didn't find one, never go further
                                  // than the end of the range.
      next_break = range.end;
    }
    std::string value = doc.substr(next_not_break, next_break);
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "[INFO]: Found KV: " << std::quoted(current_key)
              << ": " << std::quoted(value) << ". at "
              << doc.get_line_info(next_character) << std::endl;
#endif
    next_character = next_break;
    return std::make_shared<Atom>(value);
  }
  }
}

ParameterSet
parse_fhicl_document(fhicl_doc const &doc,
                     ParameterSet const &_working_set = ParameterSet(),
                     ParameterSet const &_PROLOG = ParameterSet(),
                     linedoc::doc_range range = linedoc::doc_range::whole_doc(),
                     key_t const &current_key = "") {

  bool in_prolog = false;
  std::shared_ptr<ParameterSet> PROLOG =
      std::make_shared<ParameterSet>(_PROLOG);
  std::shared_ptr<ParameterSet> working_set =
      std::make_shared<ParameterSet>(_working_set);
  std::shared_ptr<ParameterSet> ps;

  if (current_key
          .size()) { // if we have recursed into a child, the working set will
                     // contain information and the currently building parameter
                     // set should be correctly placed within it
    working_set->put(current_key, ParameterSet());
    ps = std::dynamic_pointer_cast<ParameterSet>(
        working_set->get_value_recursive(current_key));
    if (!ps) {
      throw internal_error()
          << "[ERROR]: When attempting to add working ParameterSet to "
             "working_set.";
    }
  } else { // if this is the top level, then we don't really care and
           // _working_set should be empty.
    ps = working_set;
  }

  linedoc::doc_line_point read_ptr = doc.find_first_not_of(" \n", range.begin);

  while (doc.is_earlier(read_ptr, range.end)) {

    if ((doc.get_char(read_ptr) == '#') ||
        ((doc.get_char(read_ptr) == '/') &&
         (doc.substr(read_ptr, doc.advance(read_ptr, 2)) == "//"))) {
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << "Found comment at: " << read_ptr << " "
                << std::quoted(doc.get_line(read_ptr, true)) << " from "
                << doc.get_line_info(read_ptr) << std::endl;
#endif
      // move to the next line
      read_ptr = doc.find_first_not_of(" \n", read_ptr.get_EOL());
      continue;
    }

    linedoc::doc_line_point next_char;
    linedoc::doc_line_point next_break_char =
        doc.find_first_of(" \n:", read_ptr, range.end);
    // Tokens cannot span lines
    if (doc.is_end(next_break_char)) {
      throw malformed_document()
          << "[ERROR]: Expected to find a delimited token before the end of "
             "the allowed range, from "
          << std::quoted(doc.get_line(read_ptr, true)) << " -- "
          << std::quoted(doc.get_line_info(read_ptr)) << " to "
          << std::quoted(doc.get_line(range.end, true)) << " -- "
          << std::quoted(doc.get_line_info(range.end));
    }

    std::string token = doc.substr(read_ptr, next_break_char);
    next_char = next_break_char;
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "Reading token: ("
              << std::quoted(doc.get_line(read_ptr, true)) << " -- "
              << std::quoted(doc.get_line(next_break_char, true))
              << ") = " << std::quoted(token) << std::endl;
#endif

    // A few special cases.
    if (token == "BEGIN_PROLOG") {
      if (working_set->get_names().size() || ps->get_names().size()) {
        std::stringstream ss("");
        for (auto const &name : working_set->get_names()) {
          ss << name << ": " << working_set->get_src_info(name) << std::endl;
        }
        throw malformed_document()
            << "[ERROR]: Found BEGIN_PROLOG directive at "
            << doc.get_line_info(read_ptr)
            << " after non-prolog key: value pairs have been defined: "
            << ss.str();
      }
      in_prolog = true;
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << "[INFO]: In prolog." << std::endl;
#endif
    } else if (token == "END_PROLOG") {
      in_prolog = false;
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent
                << "[INFO]: No longer in prolog: " << PROLOG->to_string()
                << std::endl;
#endif
    } else if (token.front() == '@') {
      if (token.substr(0, 6) ==
          "@table") { // @table is the only directive that is allowed
        // to be keyless and may only appear within a table context

        std::string dc =
            doc.substr(doc.advance(read_ptr, 6), doc.advance(read_ptr, 8));
        if (dc != "::") {
          throw malformed_document()
              << "[ERROR]: When reading @table directive, expected to find "
                 "double colon directive::key delimeter, but found "
              << std::quoted(doc.get_line(doc.advance(read_ptr, 6), true))
              << " -- "
              << std::quoted(doc.get_line_info(doc.advance(read_ptr, 6)));
        }

        linedoc::doc_range table_directive_key;
        table_directive_key.begin = doc.advance(read_ptr, 8);
        table_directive_key.end =
            doc.find_first_of(" \n", table_directive_key.begin, range.end);
        if (doc.is_end(table_directive_key.end)) {
          throw malformed_document()
              << "[ERROR]: Expected to find the table directive key token "
                 "before the end of the allowed range, from "
              << std::quoted(doc.get_line(table_directive_key.begin, true))
              << " -- "
              << std::quoted(doc.get_line_info(table_directive_key.begin))
              << " to " << std::quoted(doc.get_line(range.end, true)) << " -- "
              << std::quoted(doc.get_line_info(range.end));
        }

        key_t table_key = doc.substr(table_directive_key);

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
        std::cout << indent
                  << "[INFO]: Found table directive: " << std::quoted(table_key)
                  << " at "
                  << std::quoted(doc.get_line(table_directive_key.begin, true))
                  << std::endl;
#endif

        std::shared_ptr<ParameterSet> table_for_splice =
            deep_copy_resolved_reference_value<ParameterSet>(
                table_key, *working_set, *PROLOG);
        (in_prolog ? PROLOG : ps)->splice(std::move(*table_for_splice));

        next_char = table_directive_key.end;
      } else {
        throw malformed_document()
            << "[ERROR]: Found keyless fhicl directive: " << std::quoted(token)
            << " on line : " << std::quoted(doc.get_line(read_ptr, true))
            << ", but only the \"@table\" directive is allowed to appear "
               "without a key. From "
            << doc.get_line_info(read_ptr);
      }
    } else {
      // Looking for key: value
      char break_char = doc.get_char(next_break_char);

      // If we stopped because of a colon, shufty pointers one character on.
      if ((break_char == ':')) {
        next_break_char = doc.advance(next_break_char);
        token = doc.substr(read_ptr, next_break_char);
        next_char = next_break_char;
      }

      if ((token.back() != ':') && (break_char != ':')) {

        linedoc::doc_line_point next_colon =
            doc.find_first_of(":", next_break_char, range.end);

      // If you found a colon and there was only whitespace before it
        if (doc.get_char(next_colon) == ':') {
          std::string sep = doc.substr(next_break_char, doc.advance(next_colon));
          string_parsers::trim(sep);
          if (!sep.size()) {
            throw malformed_document()
                << "[ERROR]: Expected a key declaration like \"key: \", but "
                   "found "
                << std::quoted(doc.substr(read_ptr, next_colon)) << " at "
                << std::quoted(doc.get_line(next_break_char, true))
                << ". Extra whitespace between the key and the separator "
                   "should be trimmed.";
          }
        }

        throw malformed_document()
            << "[ERROR]: Expected a key declaration like \"key: \", but "
               "instead found "
            << std::quoted(token) << " at "
            << std::quoted(doc.get_line(read_ptr, true)) << " from "
            << doc.get_line_info(read_ptr);
      }

      key_t key = token.substr(0, token.size() - 1);

      std::string new_object_key =
          (current_key.size() ? (current_key + ".") : current_key) + key;

#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
      std::cout << indent << "Parsing next object from " << next_break_char
                << " = " << std::quoted(doc.get_line(next_break_char, true))
                << std::endl;
#endif

      std::shared_ptr<Base> new_obj =
          parse_object(doc, {next_break_char, range.end}, next_char,
                       *working_set, *PROLOG, new_object_key);

      (in_prolog ? PROLOG : ps)
          ->put_with_custom_history(key, std::move(new_obj),
                                    doc.get_line_info(read_ptr));
    }
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "After reading value, next_char = " << next_char
              << " = \'" << doc.get_char(next_char)
              << "\' from line: " << std::quoted(doc.get_line(next_char, true))
              << "." << std::endl;
#endif
    if (!doc.is_end(next_char)) {
      read_ptr = doc.find_first_not_of(" \n", next_char);
    } else {
      read_ptr = linedoc::doc_line_point::end();
    }
#ifdef FHICLCPP_SIMPLE_PARSERS_DEBUG
    std::cout << indent << "Next to read: " << read_ptr << " = \'"
              << doc.get_char(read_ptr)
              << "\' from line: " << std::quoted(doc.get_line(read_ptr, true))
              << "." << std::endl;
#endif
  }

  return (*ps);
}
} // namespace fhicl

#endif
