#pragma once

// gcc 9.1 will fail to build Eigen as a result of this warning if it isn't
// disabled
#if __GNUC__ >= 9
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#include "Eigen/Dense"
#pragma GCC diagnostic pop
#else
#include "Eigen/Dense"
#endif

#include "TH1.h"
#include "TH2.h"

#include <limits>
#include <vector>

Eigen::MatrixXd
GetEigenMatrix(TH2 const *,
               size_t max_rows = std::numeric_limits<size_t>::max(),
               size_t max_cols = std::numeric_limits<size_t>::max());

Eigen::VectorXd GetEigenFlatVector(std::vector<double> const &v);
Eigen::VectorXd GetEigenFlatVector(TH1 const *th);

Eigen::ArrayXd GetEigenFlatArray(std::unique_ptr<TH1> const &h);

size_t
FillHistFromEigenMatrix(TH2 *, Eigen::MatrixXd const &, size_t histx_offset = 0,
                        size_t histy_offset = 0,
                        Eigen::MatrixXd const &error = Eigen::MatrixXd());

size_t
FillHistFromEigenVector(TH2 *, Eigen::VectorXd const &, size_t vect_offset = 0,
                        size_t histx_offset = 0, size_t histy_offset = 0,
                        Eigen::VectorXd const &error = Eigen::VectorXd());
size_t
FillHistFromEigenVector(TH1 *, Eigen::VectorXd const &, size_t vect_offset = 0,
                        size_t histx_offset = 0, size_t histy_offset = 0,
                        Eigen::VectorXd const &error = Eigen::VectorXd());

template <typename THN>
inline size_t
FillHistFromEigenVector(std::vector<std::unique_ptr<THN>> &rhv,
                        Eigen::VectorXd const &vals, size_t vect_offset = 0,
                        size_t histx_offset = 0, size_t histy_offset = 0,
                        Eigen::VectorXd const &error = Eigen::VectorXd()) {

  for (std::unique_ptr<THN> const &rh : rhv) {
    vect_offset = FillHistFromEigenVector(rh.get(), vals, vect_offset,
                                          histx_offset, histy_offset, error);
  }
  return vect_offset;
}

std::vector<double> Getstdvector(TH2 const *);
std::vector<double> Getstdvector(TH1 const *);

template <typename T>
inline void Mergestdvector(std::vector<T> &a, std::vector<T> const &b) {
  for (T const &b_i : b) {
    a.push_back(b_i);
  }
}

template <typename T>
inline void Mergestdvector(std::vector<T> &a, std::vector<T> &&b) {
  for (T &b_i : b) {
    a.push_back(std::move(b_i));
  }
}

template <typename THN>
inline std::vector<double>
Getstdvector(std::vector<std::unique_ptr<THN>> const &rhv) {
  std::vector<double> ev;

  for (std::unique_ptr<THN> const &rh : rhv) {
    Mergestdvector(ev, Getstdvector(rh.get()));
  }

  return ev;
}
