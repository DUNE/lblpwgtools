#ifndef FHICLCPP_SIMPLE_TYPES_UTILITY_HXX_SEEN
#define FHICLCPP_SIMPLE_TYPES_UTILITY_HXX_SEEN

#include "types/ParameterSet.hxx"
#include "types/Sequence.hxx"
#include "types/exception.hxx"
#include "types/traits.hxx"

#include <memory>
#include <sstream>
#include <string>

namespace fhicl {

fhicl_category inline get_fhicl_category(std::shared_ptr<Base> const el) {
  if (!el) {
    return fhicl_category::kInvalidInstance;
  }
  std::shared_ptr<Atom const> atm = std::dynamic_pointer_cast<Atom const>(el);
  if (atm) {
    if (atm->is_nil()) {
      return fhicl_category::kNil;
    }
    return fhicl_category::kAtom;
  }
  std::shared_ptr<Sequence const> seq =
      std::dynamic_pointer_cast<Sequence const>(el);
  if (seq) {
    return fhicl_category::kSequence;
  }
  std::shared_ptr<ParameterSet const> ps =
      std::dynamic_pointer_cast<ParameterSet const>(el);
  if (ps) {
    return fhicl_category::kTable;
  }
  throw bizare_error()
      << "[ERROR]: When attempting to get fhicl category, failed to cast as "
         "any known type.";
}

std::string inline get_fhicl_category_string(std::shared_ptr<Base> const el) {
  fhicl_category fc = get_fhicl_category(el);
  switch (fc) {
  case fhicl_category::kInvalidInstance: {
    return "nullptr";
  }
  case fhicl_category::kNil: {
    return "@nil";
  }
  case fhicl_category::kAtom: {
    return "Atom";
  }
  case fhicl_category::kSequence: {
    return "Sequence";
  }
  case fhicl_category::kTable: {
    return "Table";
  }
  default: {
    throw bizare_error() << "[ERROR]: When attempting to get fhicl category, "
                            "failed to cast as any known type.";
  }
  }
}

inline std::shared_ptr<Base>
deep_copy_value(std::shared_ptr<Base> const original) {
  if (!original) {
    return nullptr;
  }
  std::shared_ptr<Atom const> atm =
      std::dynamic_pointer_cast<Atom const>(original);
  if (atm) {
    return std::make_shared<Atom>(*atm);
  }
  std::shared_ptr<Sequence const> seq =
      std::dynamic_pointer_cast<Sequence const>(original);
  if (seq) {
    return std::make_shared<Sequence>(*seq);
  }
  std::shared_ptr<ParameterSet const> ps =
      std::dynamic_pointer_cast<ParameterSet const>(original);
  if (ps) {
    return std::make_shared<ParameterSet>(*ps);
  }
  throw bizare_error()
      << "[ERROR]: When attempting to copy a fhicl value, failed to cast as "
         "any known type. This is an internal error, please send a full "
         "backtrace to the maintainer.";
}

} // namespace fhicl

#endif
