#pragma once

#include "CAFAnaCore/CAFAna/Core/Var.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  /// \brief Representation of a variable to be retrieved from a \ref
  /// caf::StandardRecord object
  ///
  /// A Var consists of a function, taking a StandardRecord and returning the
  /// value of the variable (which may be some complicated function).
  typedef _Var<caf::SRInteractionProxy>  Var;

//Commented things: I was trying so we could write loader.Interactions()[IntCut].{dlp,pandora}.RecoParticles().{dlp,pandora}[PartiCut]
  //  typedef _Var<caf::SRInteractionBranchProxy>  Var;

  /// \brief Equivalent of \ref Var acting on \ref caf::SRSpill
  typedef _Var<caf::SRProxy> SRVar;

  /// \brief For Vars where literally all you need is a single CAF variable
  ///
  /// eg Var myVar = SIMPLEVAR(my.var.str);
  /// NB lack of quotes quotes around my.var.str
#define SIMPLEVAR(CAFNAME) Var([](const caf::SRInteractionProxy* sr){return sr->CAFNAME;})
//#define SIMPLEVAR(CAFNAME) Var([](const caf::SRInteractionBranchProxy* sr){return sr->CAFNAME;})
#define SIMPLESRVAR(CAFNAME) SRVar([](const caf::SRProxy* sr){return sr->CAFNAME;})


  inline Var Constant(double v){return Var([v](const caf::SRInteractionProxy*){return v;});}
//  inline Var Constant(double v){return Var([v](const caf::SRInteractionBranchProxy*){return v;});}


  typedef _Var<caf::SRRecoParticleProxy> RecoPartVar;

#define SIMPLEPARTVAR(CAFNAME) RecoPartVar([](const caf::SRRecoParticleProxy* sr){return sr->CAFNAME;})

//  typedef _Var<caf::SRRecoParticlesBranchProxy> RecoPartVar;

//#define SIMPLEPARTVAR(CAFNAME) RecoPartVar([](const caf::SRRecoParticlesBranchProxy* sr){return sr->CAFNAME;})


}
