#include "CAFAna/PRISM/EigenUtils.h"

#include <iostream>

std::vector<double> Getstdvector(TH2 const *rh) {
  std::vector<double> ev;

  for (Int_t y_it = 0; y_it < rh->GetYaxis()->GetNbins(); ++y_it) {
    for (Int_t x_it = 0; x_it < rh->GetXaxis()->GetNbins(); ++x_it) {
      ev.push_back(rh->GetBinContent(x_it + 1, y_it + 1));
    }
  }

  return ev;
}

std::vector<double> Getstdvector(TH1 const *rh) {
  Int_t dim = rh->GetDimension();
  if (dim == 1) {
    std::vector<double> ev;
    for (Int_t x_it = 0; x_it < rh->GetXaxis()->GetNbins(); ++x_it) {
      ev.push_back(rh->GetBinContent(x_it + 1));
    }
    return ev;
  } else if (dim == 2) {
    return Getstdvector(static_cast<TH2 const *>(rh));
  }
  std::cout << "[ERROR]: Getstdvector cannot handle THND where N = " << dim
            << std::endl;
  throw;
}

size_t FillHistFromEigenVector(TH2 *rh, Eigen::VectorXd const &vals,
                               size_t vect_offset, size_t histx_offset,
                               size_t histy_offset,
                               Eigen::VectorXd const &error) {
  for (Int_t y_it = histy_offset; y_it < rh->GetYaxis()->GetNbins(); ++y_it) {
    for (Int_t x_it = histx_offset; x_it < rh->GetXaxis()->GetNbins(); ++x_it) {
      if (vect_offset == size_t(vals.size())) {
        return vect_offset;
      }
      rh->SetBinContent(x_it + 1, y_it + 1, vals(vect_offset));
      if (error.size()) {
        rh->SetBinError(x_it + 1, y_it + 1, error(vect_offset));
      } else {
        rh->SetBinError(x_it + 1, y_it + 1, 0);
      }
      vect_offset++;
    }
    // Reset flow bins
    rh->SetBinContent(0, y_it + 1, 0);
    rh->SetBinError(0, y_it + 1, 0);
    rh->SetBinContent(rh->GetXaxis()->GetNbins() + 1, y_it + 1, 0);
    rh->SetBinError(rh->GetXaxis()->GetNbins() + 1, y_it + 1, 0);
  }

  for (Int_t x_it = histx_offset; x_it < rh->GetXaxis()->GetNbins() + 2;
       ++x_it) {
    // Reset flow bins
    rh->SetBinContent(x_it, 0, 0);
    rh->SetBinError(x_it, 0, 0);

    rh->SetBinContent(x_it, rh->GetYaxis()->GetNbins() + 1, 0);
    rh->SetBinError(x_it, rh->GetYaxis()->GetNbins() + 1, 0);
  }
  return vect_offset;
}

size_t FillHistFromEigenVector(TH1 *rh, Eigen::VectorXd const &vals,
                               size_t vect_offset, size_t histx_offset,
                               size_t histy_offset,
                               Eigen::VectorXd const &error) {
  Int_t dim = rh->GetDimension();
  if (dim == 1) {
    for (Int_t x_it = histx_offset; x_it < rh->GetXaxis()->GetNbins(); ++x_it) {
      if (vect_offset == size_t(vals.size())) {
        return vect_offset;
      }
      rh->SetBinContent(x_it + 1, vals(vect_offset));
      if (error.size()) {
        rh->SetBinError(x_it + 1, error(vect_offset));
      } else {
        rh->SetBinError(x_it + 1, 0);
      }
      vect_offset++;
    }
    // Reset flow bins
    rh->SetBinContent(0, 0);
    rh->SetBinError(0, 0);
    rh->SetBinContent(rh->GetXaxis()->GetNbins() + 1, 0);
    rh->SetBinError(rh->GetXaxis()->GetNbins() + 1, 0);
    return vect_offset;
  } else if (dim == 2) {
    return FillHistFromEigenVector(static_cast<TH2 *>(rh), vals, vect_offset,
                                   histx_offset, histy_offset, error);
  }
  std::cout << "[ERROR]: FillHistFromEigenVector cannot handle THND where N = "
            << dim << std::endl;
  throw;
}

Eigen::MatrixXd GetEigenMatrix(TH2 const *h, size_t max_rows, size_t max_cols) {
  Int_t NRows = std::min(max_rows, size_t(h->GetNbinsY()));
  Int_t NCols = std::min(max_cols, size_t(h->GetNbinsX()));

  Eigen::MatrixXd em(NRows, NCols);
  for (Int_t ir = 0; ir < em.rows(); ++ir) {
    for (Int_t ic = 0; ic < em.cols(); ++ic) {
      em(ir, ic) = h->GetBinContent(ic + 1, ir + 1);
    }
  }
  return em;
}

size_t FillHistFromEigenMatrix(TH2 *h, Eigen::MatrixXd const &em,
                               size_t histx_offset, size_t histy_offset,
                               Eigen::MatrixXd const &error) {

  bool have_errors = (error.rows() == em.rows()) && (error.cols() == em.cols());

  size_t ctr = 0;
  for (int ir = histy_offset; ir < em.rows(); ++ir) {
    for (int ic = histx_offset; ic < em.cols(); ++ic) {
      h->SetBinContent(ic + 1, ir + 1, em(ir, ic));
      if (have_errors) {
        h->SetBinError(ic + 1, ir + 1, error(ir, ic));
      }
      ctr++;
    }
  }
  return ctr;
}

Eigen::VectorXd GetEigenFlatVector(std::vector<double> const &v) {
  Eigen::VectorXd ev(v.size());
  size_t idx = 0;
  size_t N = v.size();
  for (size_t i = 0; i < N; ++i) {
    ev(idx++) = v[i];
  }
  return ev;
}
Eigen::VectorXd GetEigenFlatVector(TH1 const *th) {
  return GetEigenFlatVector(Getstdvector(th));
}

Eigen::ArrayXd GetEigenFlatArray(std::unique_ptr<TH1> const &h) {
  int N = h->GetXaxis()->GetNbins();
  Eigen::ArrayXd arr(N); //= Eigen::ArrayXd::Zero(N);
  for (int i = 0; i < N; i++) {
    arr(i) = h->GetBinContent(i + 1);
  } 
  return arr;
}
