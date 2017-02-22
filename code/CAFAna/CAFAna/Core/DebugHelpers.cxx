// This file sets things up (when libCAFAnaCore is loaded) so that we invoke
// gdb to get a stack trace after any unusual program termination. You can
// suppress this behaviour by setting the CAFE_NO_BACKTRACE environment
// variable.

#include <csignal>
#include <cstring>

#include <unistd.h>

#include <iostream>

namespace ana
{
  void gdb_backtrace()
  {
    char s[1024];
    sprintf(s, "gdb --batch -ex sharedlibrary -ex bt root.exe %d", getpid());
    system(s);
  }

  bool gIsException = false;

  void handle_terminate()
  {
    std::cout << "\n\nUncaught exception\n" << std::endl;
    gIsException = true;
    // Next thing that happens is abort(), which goes to handle_signal()
  }

  void handle_signal(int sig, siginfo_t*, void*)
  {
    if(sig == SIGABRT && !gIsException)
      std::cout << "\n\nAborted\n" << std::endl;
    if(sig == SIGSEGV)
      std::cout << "\n\nSegmentation fault\n" << std::endl;
    if(sig == SIGFPE)
      std::cout << "\n\nFloating point exception\n" << std::endl;
    if(sig == SIGBUS)
      std::cout << "\n\nBus error\n" << std::endl;

    gdb_backtrace();

    if(sig != SIGABRT) abort();
  }

  class InstallHandlers
  {
  public:
    InstallHandlers()
    {
      // We're already being debugged somehow. Don't complicate things
      if(getenv("CAFE_NO_BACKTRACE")) return;

      // Handle uncaught exceptions
      std::set_terminate(handle_terminate);

      // Handle abort(), segfaults, bus errors, and FPEs
      struct sigaction sa;
      memset(&sa, 0, sizeof(sa));
      sigemptyset(&sa.sa_mask);

      sa.sa_sigaction = handle_signal;
      sa.sa_flags = SA_SIGINFO;

      sigaction(SIGABRT, &sa, NULL);
      sigaction(SIGSEGV, &sa, NULL);
      sigaction(SIGBUS, &sa, NULL);
      sigaction(SIGFPE, &sa, NULL);
    }
  };

  static InstallHandlers gHandlerInstaller;
}
