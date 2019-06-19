#pragma once

#include "CAFAna/Core/HistAxis.h"

#include <string>

struct AxisBlob {
  ana::HistAxis const *NDAx;
  ana::HistAxis const *FDAx_numu;
  ana::HistAxis const *FDAx_nue;
};

extern AxisBlob const default_axes_v4;
extern AxisBlob const default_axes_v3;

AxisBlob GetAxisBlob(std::string const &);
