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
  Eigen::ArrayXd arr(N + 2); 
  for (int i = 0; i <= N + 1; i++) {
    arr(i) = h->GetBinContent(i);
  } 
  return arr;
}

Eigen::MatrixXd ConvertArrayToMatrix(Eigen::ArrayXd const &arr, 
                                     std::vector<ana::Binning> const &bins) {
  Eigen::MatrixXd ret_mat;

  if (bins.size() == 2) { // 2D axiis.
    int NRows = bins.at(1).NBins();
    int NCols = bins.at(0).NBins();
    ret_mat.resize(NRows + 2, NCols + 2);
    ret_mat.setZero();
    for (int col = 1; col <= NCols; col++) {
      for (int row = 1; row <= NRows; row++) {
        ret_mat(row, col) = arr(row + (col - 1) * NRows);
      }
    }
  } else if (bins.size() == 3) {
    // implement 3D axis another day.
    // 3D means first 2 axis are the analysis axes and the third is the weighting axis,
    // which for PRISM is typically off-axis position.
    int NRows = bins.at(2).NBins();
    int NCols1 = bins.at(0).NBins();
    int NCols2 = bins.at(1).NBins();
    int NColsTotal = bins.at(0).NBins() * bins.at(1).NBins();
    ret_mat.resize(NRows + 2, NColsTotal + 2);
    ret_mat.setZero();
    for (int row = 1; row <= NRows; row++) {
      for (int col2 = 1; col2 <= NCols2; col2++) {
        for (int col1 = 1; col1 <= NCols1; col1++) {
          int colit = col2 + (col1 - 1) * NCols2;
          ret_mat(row, colit) = arr(row + (colit - 1) * NRows); 
        }
      }
    }
  }
  return ret_mat;
}
