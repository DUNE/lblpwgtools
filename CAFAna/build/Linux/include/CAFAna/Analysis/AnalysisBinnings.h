#pragma once

#include "CAFAna/Core/HistAxis.h"

#include <string>

extern ana::Binning const binsNDEreco;
extern ana::Binning const binsFDEreco;

struct AxisBlob {
  ana::HistAxis const *NDAx;
  ana::HistAxis const *FDAx_numu;
  ana::HistAxis const *FDAx_nue;
};

AxisBlob GetAxisBlob(std::string const &);
