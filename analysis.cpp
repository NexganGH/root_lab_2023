#include "DefaultParticles.hpp"
#include "Particle.hpp"

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

void PrintParticleTypes(TH1F *histo_particles) {
  auto total_n = histo_particles->GetEntries();
  // std::cout << "Total entries: " << total_n << std::endl;
  for (int i = 0; i < histo_particles->GetNbinsX(); i++) {

    // std::cout << "Particle: " << i << std::endl;
    double n_particle_type = histo_particles->GetBinContent(i + 1);
    // std::cout << "after particle type";
    double n_particle_error = histo_particles->GetBinError(i + 1);
    // std::cout << "after error";
    auto type = Particle::getParticle(i);

    // std::cout << "after type";
    auto type_percentage = n_particle_type / total_n;
    auto error_percentage = n_particle_error / total_n;

    std::cout << "Particle " << type->getFName() << ": " << type_percentage
              << ", error: " << error_percentage << std::endl;
  }
}

void PrintAngularFit(TH1D *histo_angle_px, TH1D *histo_angle_py) {

  auto print_single_angle = [](TH1D *histo_angle) {
    TF1 *f = new TF1("function_angle", "[0]", 0, 2 * M_PI);
    histo_angle->Fit(f, "Q");
    std::cout << "f(x) = " << f->GetParameter(0)
              << ", parameter error = " << f->GetParError(0) << std::endl;
    std::cout << "Chi square/NDF = " << f->GetChisquare() / f->GetNDF()
              << std::endl;

    std::cout << "Fit probability: " << f->GetProb() << std::endl;
  };

  std::cout << "--- Angle px --- " << std::endl;
  print_single_angle(histo_angle_px);

  std::cout << "--- Angle py --- " << std::endl;
  print_single_angle(histo_angle_py);
}

void PrintPModuleFit(TH1F *histo_p_module) {

  std::cout << "--- P Module ---" << std::endl;
  TF1 *f = new TF1("function_p_module", "[0]*exp(-[1] * x)");
  histo_p_module->Fit(f, "Q");
  std::cout << "f(x) = " << f->GetParameter(0) << " * e^(-"
            << f->GetParameter(1) << "*x)" << std::endl;
  std::cout << "Parameter errors (in order) = " << std::endl;

  for (int i = 0; i < 2; i++) {
    std::cout << f->GetParError(i) << ", ";
  }
  std::cout << std::endl;
  std::cout << "Chi square/NDF = " << f->GetChisquare() / f->GetNDF()
            << std::endl;

  std::cout << "Fit probability: " << f->GetProb() << std::endl;
}

TH1F *SubtractInvMass(TH1F *disc_histo, TH1F *conc_histo) {
  TH1F *res = new TH1F(*disc_histo);
  res->Add(conc_histo, -1);

  return res;
}

void Analysis() {
  DefaultParticles::Load();

  TFile *file = new TFile("save.root");

  auto *histo_particle_types = file->Get<TH1F>("histo_particelle");

  auto *histo_P_module = file->Get<TH1F>("histo_P_module");
  auto *histo_angle = file->Get<TH2F>("histo_phi_theta");

  auto *histo_angle_px = file->Get<TH1D>("histo_phi_theta_px");

  auto *histo_angle_py = file->Get<TH1D>("histo_phi_theta_py");

  auto *histo_impulso_trasverso = file->Get<TH1F>("histo_impulso_trasverso");
  auto *histo_energia = file->Get<TH1F>("histo_energia");

  auto *inv_mass = file->Get<TH1F>("inv_mass");

  auto *disc_inv_mass = file->Get<TH1F>("disc_inv_mass");
  auto *conc_inv_mass = file->Get<TH1F>("conc_inv_mass");

  auto *p_pos_k_neg = file->Get<TH1F>("p_pos_k_neg");
  auto *p_pos_k_pos = file->Get<TH1F>("p_pos_k_pos");

  auto *k_star_dec = file->Get<TH1F>("k_star_dec");

  PrintParticleTypes(histo_particle_types);
  PrintAngularFit(histo_angle_px, histo_angle_py);
  PrintPModuleFit(histo_P_module);

  TCanvas *canvas1 = new TCanvas();

  canvas1->Divide(2, 2);

  canvas1->cd(1);
  histo_particle_types->DrawCopy();

  canvas1->cd(2);
  histo_P_module->DrawCopy();

  canvas1->cd(3);
  histo_impulso_trasverso->DrawCopy();

  canvas1->cd(4);
  histo_energia->DrawCopy();

  auto canvas2 = new TCanvas();
  canvas2->Divide(2, 2);

  canvas2->cd(1);
  histo_angle->DrawCopy();

  canvas2->cd(2);
  histo_angle_px->DrawCopy();

  canvas2->cd(3);
  histo_angle_py->DrawCopy();

  auto *canvas3 = new TCanvas();
  canvas3->Divide(2, 3);

  canvas3->cd(1);
  inv_mass->DrawCopy();

  canvas3->cd(2);
  disc_inv_mass->DrawCopy();

  canvas3->cd(3);
  conc_inv_mass->DrawCopy();

  canvas3->cd(4);
  p_pos_k_neg->DrawCopy();

  canvas3->cd(5);
  p_pos_k_pos->DrawCopy();

  canvas3->cd(6);
  k_star_dec->DrawCopy();

  // CANVAS 4
  // auto *canvas4 =
  //     new TCanvas("canvas_invariant_mass_subtraction",
  //                 "Analysis: subtraction of invariant masses", 3, 1);

  auto *canvas4 = new TCanvas();
  canvas4->Divide(2, 2);
  auto total = SubtractInvMass(disc_inv_mass, conc_inv_mass);
  canvas4->cd(1);
  total->DrawCopy("P");
  total->Fit("gaus");
  total->DrawCopy("P");
  // histo_energia->DrawCopy();

  auto pioni_kaoni = SubtractInvMass(p_pos_k_neg, p_pos_k_pos);
  canvas4->cd(2);
  pioni_kaoni->DrawCopy("P");
  pioni_kaoni->Fit("gaus");
  pioni_kaoni->DrawCopy("P");
  // histo_angle->DrawCopy();

  // canvas

  canvas4->cd(3);
  k_star_dec->DrawCopy();

  file->Close();
}