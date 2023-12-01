#include "Particle.hpp"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

void PrintParticleTypes(TH1F *histo_particles) {
  auto total_n = histo_particles->GetEntries();
  std::cout << "Total entries: " << total_n << std::endl;
  for (int i = 0; i < histo_particles->GetNbinsX(); i++) {

    std::cout << "Particle: " << i << std::endl;
    double n_particle_type = histo_particles->GetBinContent(i + 1);
    std::cout << "after particle type";
    double n_particle_error = histo_particles->GetBinError(i + 1);
    std::cout << "after error";
    auto type = Particle::getParticle(i);

    std::cout << "after type";
    auto type_percentage = n_particle_type / total_n;
    auto error_percentage = n_particle_error / total_n;

    std::cout << "Particle " << type->getFName() << ": " << type_percentage
              << ", error: " << error_percentage << std::endl;
  }
}

void Analysis() {
  TFile *file = new TFile("save.root");

  // file->ls();

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
  auto *p_pos_k_pos = file->Get<TH1F>("p_pos_k_neg");

  auto *k_star_dec = file->Get<TH1F>("k_star_dec");

  PrintParticleTypes(histo_particle_types);

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

  file->Close();
}