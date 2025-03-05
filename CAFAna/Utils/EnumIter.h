#pragma once

// this file's contents borrowed from https://medium.com/@ryan_forrester_/c-enum-iteration-comprehensive-guide-647a30f0f2ca

#include <iostream>
#include <type_traits>

namespace ana
{
  template <typename E>
  constexpr auto to_underlying(E e) noexcept
  {
      return static_cast<std::underlying_type_t<E>>(e);
  }

  template <typename E>
  class EnumIterator
  {
    private:
      int value;

    public:
      explicit EnumIterator(int v) : value(v) {}
      E operator*() const { return static_cast<E>(value); }
      EnumIterator& operator++() { ++value; return *this; }
      bool operator!=(const EnumIterator& other) const { return value != other.value; }
  };

  template <typename E>
  class EnumRange
  {
    private:
      int begin_value, end_value;

    public:
      EnumRange(E begin, E end) : begin_value(to_underlying(begin)), end_value(to_underlying(end)) {}
      EnumIterator<E> begin() const { return EnumIterator<E>(begin_value); }
      EnumIterator<E> end() const { return EnumIterator<E>(end_value + 1); }
  };

  template <typename E>
  EnumRange<E> enum_range(E begin, E end)
  {
      return EnumRange<E>(begin, end);
  }

#define LAST_ELEM(E)  kNumEs
}