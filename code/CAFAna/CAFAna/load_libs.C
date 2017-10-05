// -*- mode: c++; c-basic-offset: 2; -*-
void load(std::string lib)
{
  std::cout << "." << std::flush;
  int ret = gSystem->Load(("lib"+lib).c_str());
  // In case of error, exit immediately with the error clearly showing, instead
  // of with a confusing secondary error about a page of output later.
  if(ret != 0) exit(1);
}

void load_libs()
{
  // All the CINT exception handler does is obfuscate the stack. With this,
  // uncaught exceptions immediately show a useful backtrace under gdb.
  //  G__SetCatchException(0);

  TString qsrt = gSystem->Getenv("SRT_QUAL");
  TString qmrb = gSystem->Getenv("MRB_QUALS");
  // Mirror the optimization settings in use elsewhere
  if( qsrt.Contains("debug") || qmrb.Contains("debug") ) {
    gSystem->SetAclicMode(TSystem::kDebug);
  }
  else{
    gSystem->SetAclicMode(TSystem::kOpt);
  }

  // This magic incantation prevents ROOT doing slow cleanup work in
  // TList::RecursiveRemove() under ~TH1(). It also tends to lead to shutdown
  // crashes. HistCache now avoids almost all of that histogram destruction, so
  // now we can leave this at the default setting and get both speed and
  // stability.
  //  gROOT->SetMustClean(false);

  // Colorize error messages. Would be better if we could just pick up the
  // flags novasoft uses, but they don't seem to be in any env var.
  gSystem->SetFlagsDebug(TString(gSystem->GetFlagsDebug())+" -fdiagnostics-color=auto");
  gSystem->SetFlagsOpt(TString(gSystem->GetFlagsOpt())+" -fdiagnostics-color=auto");


  // Include path
  TString includes = "-I$SRT_PRIVATE_CONTEXT/include/ -I$SRT_PUBLIC_CONTEXT/include/ -I$ROOTSYS/include -I$NUTOOLS_INC -I$GENIE_INC/GENIE/";

  if (qmrb == "") {    // This is the SRT build
    // List of libraries to load. Dependency order.
    const int kNumLibs = 13;//20;
    const std::string libs[kNumLibs] =
      {
        "Minuit2", // CAFReweight pulls in Genie which pulls in ROOT geometry
        // "Cintex",
        "StandardRecord",
        // "StandardRecord_dict",
        "CAFAnaCore",
        "CAFAnaVars",
        "CAFAnaCuts",
        "CAFAnaSysts",
        // "CAFAnaUnfold",
        "CAFAnaDecomp",
        "CAFAnaExtrap",
        "CAFAnaPrediction",
        "CAFAnaExperiment",
        // "CAFAnaFC",
        "CAFAnaAnalysis",
        // "CAFAnaXSec",
        // "CAFAna",
        "OscLibFunc",
        // "MCReweightFunc",
        "ifdh"
      };

    // Actually load the libraries
    std::cout << "Loading libraries";
    for(unsigned int i = 0; i < kNumLibs; ++i) load(libs[i]);
    std::cout << std::endl;
  }
  else {   // This is the MRB build
    // List of libraries to load. Dependency order.
    const int kNumLibs = 10;
    const std::string libs[kNumLibs] =
      {
        "Geom", "Tree", "Minuit2", // CAFReweight pulls in Genie which pulls in ROOT geometry
        "Cintex",
        "StandardRecord", "StandardRecord_dict",
        "CAFAna",
        "OscLibFunc",
	"MCReweightFunc",
        "ifdh"
      };

    // Actually load the libraries
    std::cout << "Loading libraries (mrb)";
    for(unsigned int i = 0; i < kNumLibs; ++i) load(libs[i]);
    std::cout << std::endl;

    // MRB requires an extra include path
    includes += " -I$NOVASOFT_INC";
  }

  // Magic incantation to get Reflex-based dictionaries converted into the
  // CINT-based information that ROOT needs
  //  Cintex::Enable();


  gSystem->SetIncludePath(includes);

  // Doesn't seem to work
  //  gSystem->Setenv("IFDH_DEBUG", "0"); // shut ifdh up

  // Pick up standard NOvA style
  gROOT->LoadMacro("$SRT_PUBLIC_CONTEXT/Utilities/rootlogon.C");
  //  rootlogon();
  gROOT->ForceStyle();

  TRint* rint = dynamic_cast<TRint*>(gApplication);
  if(rint) rint->SetPrompt("cafe [%d] ");


  // Do this last so that the profiler library is unloaded before our main
  // libraries, meaning that the code in ProfilerSupport runs at the right time
  // (after the profile file is made). This does mean that we aren't able to
  // profile library loading/startup. The ideal solution would be to make a
  // mini-library for just ProfilerSupport.
  if(gSystem->Getenv("CPUPROFILE")){
    // We were passed the --prof option
    const std::string cpuprof = gSystem->Getenv("CPUPROFILE");

    std::cout << "Profiling enabled." << std::endl;

    if(!qsrt.Contains("debug") && !qmrb.Contains("debug")){
      std::cout << "Note: profiling works much better in debug mode." << std::endl;
    }

    // Caltech location
    if(gSystem->AccessPathName("/nfs/raid11/novasoft/externals/lib/libprofiler.so") == false) // yes, bizarre convention...
      gSystem->Load("/nfs/raid11/novasoft/externals/lib/libprofiler.so");

    // Fermilab location
    if(gSystem->AccessPathName("/grid/fermiapp/nova/perftools/lib/libprofiler.so") == false)
      gSystem->Load("/grid/fermiapp/nova/perftools/lib/libprofiler.so");

    // Would be much better to have it packaged as a ups product of course

    // Apparently the library load manages to corrupt the env var? Put it back.
    gSystem->Setenv("CPUPROFILE", cpuprof.c_str());
  }
}
