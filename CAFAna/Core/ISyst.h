#pragma once

#include "cafanacore/ISyst.h"

#include <string>

#include <list>

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  /// \brief Encapsulate code to systematically shift a \ref
  /// caf::StandardRecord
  ///
  /// The Shift() function alters the \ref caf::StandardRecord or the weight
  /// associated with the event.
  //template<class RecT>
  //class ISyst: public _ISyst<RecT>
  class ISyst: public _ISyst<caf::SRInteractionProxy>
  {
  public:
    ISyst(const std::string& shortName,
          const std::string& latexName,
	        bool applyPenalty = true,
          double min = -3,
          double max = +3);
    //  	  double cv = 0);
    //ISyst(const ISyst &) = delete;   // no copying.
    //ISyst(ISyst && rhs) = delete;    // no moving either.
    virtual ~ISyst();

    // void operator=(const ISyst &) = delete;  // still no copying.
    //void operator=(ISyst &&)      = delete;  // etc.

    virtual double Penalty(double x) const;

    /// Should a penalty be applied for this shift?
    virtual bool ApplyPenalty() const {return fApplyPenalty;}

    /// Return the min/max value for this syst
    virtual double Min() const{return fMin;}
    virtual double Max() const{return fMax;}

    ///// Set the central value used
    // virtual void SetCentral(double val) const{fCentral = val;}

    ///// Return the central value used
    //virtual double Central() const{return fCentral;}

    // Is this even usegul?
    ///// \brief Perform the systematic shift
    /////
    ///// \param sigma   Number of sigma to shift record by
    ///// \param sr      The record to inspect and alter
    ///// \param weight  Scale this weight for reweighting systematics
    //virtual void Shift(double sigma,
    //                   caf::SRProxy* sr,
    //                   double& weight) const = 0;

    /// PredictionInterp normally interpolates between spectra made at
    /// +/-1,2,3sigma. For some systematics that's overkill. Override this
    /// function to specify different behaviour for this systematic.
    virtual int PredInterpMaxNSigma() const
    {
      return 3;
    }

  private:
    bool fApplyPenalty;
    double fMin;
    double fMax;
    //mutable double fCentral; // I make no apologies for this
  };

} // namespace
