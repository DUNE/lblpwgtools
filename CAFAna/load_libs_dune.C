// -*- mode: c++; c-basic-offset: 2; -*-
void load(std::string lib) {
  int ret = gSystem->Load(("lib" + lib).c_str());
  std::cout << "\tLoading lib: " << ("lib" + lib) << std::endl;
  // In case of error, exit immediately with the error clearly showing, instead
  // of with a confusing secondary error about a page of output later.
  if (ret != 0) {
    std::cout << std::endl
              << "gSystem->Load(lib" + lib + ") failed with code " << ret
              << std::endl;
    exit(ret);
  }
}

void load_libs(bool MustClean = true) {
  // All the CINT exception handler does is obfuscate the stack. With this,
  // uncaught exceptions immediately show a useful backtrace under gdb.
  //  G__SetCatchException(0);

  gSystem->SetAclicMode(TSystem::kOpt);

  // This magic incantation prevents ROOT doing slow cleanup work in
  // TList::RecursiveRemove() under ~TH1(). It also tends to lead to shutdown
  // crashes. HistCache now avoids almost all of that histogram destruction, so
  // now we can leave this at the default setting and get both speed and
  // stability.
  std::cout << "gROOT->SetMustClean(" << (MustClean ? "true" : "false") << ");"
            << std::endl;
  gROOT->SetMustClean(MustClean);

  // Colorize error messages. Would be better if we could just pick up the
  // flags novasoft uses, but they don't seem to be in any env var.
  gSystem->SetFlagsDebug(
      TString(gSystem->GetFlagsDebug()) +
      " -fdiagnostics-color=auto -DDONT_USE_FQ_HARDCODED_SYST_PATHS=1");
  gSystem->SetFlagsOpt(
      TString(gSystem->GetFlagsOpt()) +
      " -fdiagnostics-color=auto -UNDEBUG "
      "-DDONT_USE_FQ_HARDCODED_SYST_PATHS=1"); // match gcc's maxopt behaviour
                                               // of retaining assert()

  // Include path
  TString includes = "-I$ROOTSYS/include -I$CAFANA/include -I$STAN_INC -I$STAN_MATH_INC -I$EIGEN_INC -I$SUNDIALS_INC";

  const std::vector<std::string> libs = {
      "Minuit2",          "Net",           "Tree", "StandardRecord", "OscLibFunc",
      "UtilitiesFunc",    "CAFAnaCore",    "CAFAnaVars",     "CAFAnaCuts",
      "CAFAnaExperiment", "CAFAnaSysts",   "CAFAnaDecomp",   "CAFAnaExtrap",
      "CAFAnaPrediction", "CAFAnaFit", "CAFAnaAnalysis", "boost_filesystem",
      "boost_system"};

  // Actually load the libraries
  std::cout << "Loading libraries:" << std::endl;
  for (const std::string &lib : libs)
    load(lib);
  std::cout << std::endl;

  // Magic incantation to get Reflex-based dictionaries converted into the
  // CINT-based information that ROOT needs
  //  Cintex::Enable();

  gSystem->SetIncludePath(includes);

  // Pick up standard NOvA style
  gROOT->Macro("$CAFANA/include/Utilities/rootlogon.C");
  gROOT->ForceStyle();

  TRint *rint = dynamic_cast<TRint *>(gApplication);
  if (rint) {
    rint->SetPrompt("cafe [%d] ");
  }
}
