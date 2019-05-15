// Copyright 2019 Hubert andre

#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkPen.h>
#include <vtkPlot.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTable.h>
#include <iostream>
#include <random>
#include "ceres/ceres.h"

double model(double x) { return exp(0.3 * x + 0.1); }
template <typename T>
struct GaussModel {
  T mu_;
  T sig_;
};
template <typename T>
struct Data {
  std::vector<T> x_;
  std::vector<T> y_;
};
template <typename T>
struct UserSynth {
  T x_min_;
  T x_max_;
  int n;
};

template <typename T>
class DataSynthetizer {
 private:
  UserSynth<T> user_params_;
  Data<T> data_noisy_;
  Data<T> data_pure_;
  Data<T> noise_;
  T (*function_)(T x);
  GaussModel<T> gauss_model_;

 public:
  DataSynthetizer(UserSynth<T> user_params, T (*function)(T x),
                  GaussModel<T> gauss_model);
  Data<T> *GetNoisy() { return &data_noisy_; }
  UserSynth<T> GetUserSynth() { return user_params_; }
};

template <typename T>
DataSynthetizer<T>::DataSynthetizer(UserSynth<T> user_params,
                                    T (*function)(T x),
                                    GaussModel<T> gauss_model)
    : user_params_(user_params),
      function_(function),
      gauss_model_(gauss_model) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<T> real_normal(gauss_model_.mu_, gauss_model_.sig_);
  std::uniform_real_distribution<T> real_uniform(user_params_.x_min_,
                                                 user_params_.x_max_);
  data_noisy_.x_.reserve(user_params_.n);
  data_noisy_.y_.reserve(user_params_.n);
  data_pure_.x_.reserve(user_params_.n);
  data_pure_.y_.reserve(user_params_.n);
  noise_.x_.reserve(user_params_.n);
  noise_.y_.reserve(user_params_.n);

  for (int i = 0; i < user_params.n; i++) {
    data_pure_.x_.push_back(real_uniform(gen));
    data_pure_.y_.push_back(function_(data_pure_.x_[i]));
    noise_.x_.push_back(real_normal(gen));
    noise_.y_.push_back(real_normal(gen));
    data_noisy_.x_.push_back(data_pure_.x_[i] + noise_.x_[i]);
    data_noisy_.y_.push_back(data_pure_.y_[i] + noise_.y_[i]);
  }
}

template <typename T>
class Plotter {
 private:
  vtkSmartPointer<vtkTable> table_;
  vtkSmartPointer<vtkDoubleArray> arrX_;
  DataSynthetizer<T> data_synth_;
  vtkSmartPointer<vtkDoubleArray> arrY_;
  vtkSmartPointer<vtkContextView> view_;
  vtkSmartPointer<vtkChartXY> chart_;
  vtkPlot *points_;

 public:
  explicit Plotter(DataSynthetizer<T> data_synth) : data_synth_(data_synth) {
    table_ = vtkSmartPointer<vtkTable>::New();
    arrX_ = vtkSmartPointer<vtkDoubleArray>::New();
    arrX_->SetName("X Axis");
    arrX_->SetArray(&data_synth_.GetNoisy()->x_[0],
                    data_synth_.GetUserSynth().n, 1);
    table_->AddColumn(arrX_);
    arrY_ = vtkSmartPointer<vtkDoubleArray>::New();
    arrY_->SetName("Data");
    arrY_->SetArray(&data_synth_.GetNoisy()->y_[0],
                    data_synth_.GetUserSynth().n, 1);
    table_->AddColumn(arrY_);
    view_ = vtkSmartPointer<vtkContextView>::New();
    view_->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
    chart_ = vtkSmartPointer<vtkChartXY>::New();
    view_->GetScene()->AddItem(chart_);
    points_ = chart_->AddPlot(vtkChart::POINTS);
    points_->SetInputData(table_, 0, 1);
    points_->SetColor(0, 255, 0, 255);
    points_->SetWidth(1.0);
  }
  void plot() {
    view_->GetRenderWindow()->Render();
    view_->GetInteractor()->Initialize();
    view_->GetInteractor()->Start();
  }
};

int main(int argc, char const *argv[]) {
  double xmin = 0;
  double xmax = 4;
  int data_n = 10;
  UserSynth<double> user_params{xmin, xmax, data_n};
  double sigma = 0.1;
  double mu = 0;
  GaussModel<double> gm{mu, sigma};
  DataSynthetizer<double> data_synth(user_params, model, gm);

  Plotter<double> plotter(data_synth);
  plotter.plot();
  return 0;
}
