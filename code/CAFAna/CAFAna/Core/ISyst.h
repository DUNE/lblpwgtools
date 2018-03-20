#pragma once

#include <list>
#include <set>
#include <string>

namespace caf{class StandardRecord;}

namespace ana
{
  class Restorer;

  /// \brief Encapsulate code to systematically shift a \ref
  /// caf::StandardRecord
  ///
  /// The Shift() function alters the \ref caf::StandardRecord or the weight
  /// associated with the event. You must be careful to add any variables you
  /// modify to \ref Restorer (before you modify them!) so that the framework
  /// can undo your systematic shift before using the record elsewhere.
  class ISyst
  {
  public:
    ISyst();
    virtual ~ISyst();

    /// The name printed out to the screen
    virtual std::string ShortName() const = 0;

    /// The name used on plots (ROOT's TLatex syntax)
    virtual std::string LatexName() const = 0;

    /// \brief Perform the systematic shift
    ///
    /// \param sigma   Number of sigma to shift record by
    /// \param restore Add fields here before you alter them
    /// \param sr      The record to inspect and alter
    /// \param weight  Scale this weight for reweighting systematics
    virtual void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const = 0;

    /// GENIE reweights can only provide +/-1,2sigma
    virtual bool IsGenieReweight() const {return false;}

    /// PredictionInterp normally interpolates between spectra made at
    /// +/-1,2,3sigma. For some systematics that's overkill. Override this
    /// function to specify different behaviour for this systematic.
    virtual int PredInterpMaxNSigma() const
    {
      return IsGenieReweight() ? 2 : 3;
    }
  };


  /// \brief Store a list of variables and restore their values when destructed
  ///
  /// It's too costly to keep an unaltered copy of StandardRecord and copy it
  /// each time a systematically shifted spectrum is needed (StandardRecord is
  /// large). Instead we have to reset just the fields that have been
  /// touched. This class allows systematic shifts to do that with the minimum
  /// of boilerplate.
  class Restorer
  {
  public:
    /// All variables passed to \ref Add will be rest at destruction
    ~Restorer()
    {
      // Put everything back. Add() ensured that the initial value we finally
      // need to get back to for each variable is stored after its other
      // values.
      for(auto it: fFloats) *it.first = it.second;
      for(auto it: fDoubles) *it.first = it.second;
      for(auto it: fInts  ) *it.first = it.second;
      for(auto it: fBools ) *it.first = it.second;
    }

    // Restoration runs front to back, so put newest entries on the front so
    // that oldest entries eventually win.
    void Add(float&  f){fFloats.emplace_front(&f, f);}
    void Add(double& d){fDoubles.emplace_front(&d, d);}
    void Add(int&    i){fInts  .emplace_front(&i, i);}
    void Add(bool&   b){fBools .emplace_front(&b, b);}

    /// Can specify many fields of different types in one call
    template<class T, class... U> void Add(T& x, U&... xs)
    {
      // Add the first one based on one of the specializations above
      Add(x);
      // Recurse to handle the rest
      Add(xs...);
    }

    bool Empty() const
    {
      return fFloats.empty() && fInts.empty() && fBools.empty() && fDoubles.empty();
    }

  protected:
    // I believe these are all the variable types existing in StandardRecord If
    // there's one missing, it's easy to add. Too few to be worth figuring out
    // a template way of doing this.
    std::list<std::pair<float*, float>> fFloats;
    std::list<std::pair<double*, double>> fDoubles;
    std::list<std::pair<int*,   int  >> fInts;
    std::list<std::pair<bool*,  bool >> fBools;
  };
} // namespace
