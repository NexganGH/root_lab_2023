#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

void PrintParticleTypes(TH1F *histo_particles) {
  for (int i = 0; i < histo_particles->GetNbinsX(); i++) {
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

  //   TH1F *histo_P_module =
  //       new TH1F("histo_P_module", "P_module", 100, 0.,
  //                1); // ho scelto il numero dei bin a caso e il limite
  //                    // superiore a caso perchè dovrebbe essere genera da
  //                    // distribuzioni eponenziali
  //   // TH1F *histo_phi = new TH1F("histo_phi", "phi", 100, 0., 2 * M_PI);
  //   // TH1F *histo_theta = new TH1F("histo_theta", "theta", 100, 0., M_PI);
  //   TH2F *histo_angle = new TH2F("histo_phi_theta", "Phi & Theta plot", 100,
  //   0,
  //                                2 * M_PI, 100, 0, M_PI);
  //   TH1F *histo_impulso_trasverso =
  //       new TH1F("histo_impulso_trasverso", "impulso_trasverso", 100, 0., 1);
  //   TH1F *histo_energia = new TH1F("histo_energia", "Energia", 100, 0., 3);

  //   // TH1F *hiso_theta = new TH1F("histo_theta", "theta", 100, 0., M_PI);
  //   // TH1F *hiso_theta = new TH1F("histo_theta", "theta", 100, 0., M_PI);
  //   TH1F *inv_mass = new TH1F("inv_mass", "Invariant mass", 100, 0., 3);

  //   TH1F *disc_inv_mass =
  //       new TH1F("disc_inv_mass", "Discordant invariant mass", 100, 0., 3);
  //   TH1F *conc_inv_mass =
  //       new TH1F("conc_inv_mass", "Concordant invariant mass", 100, 0., 3);

  //   TH1F *p_pos_k_neg =
  //       new TH1F("p_pos_k_neg", "Pione+/Kaone- e Pione-/Kaone+", 100, 0., 3);
  //   TH1F *p_pos_k_pos =
  //       new TH1F("p_pos_k_neg", "Pione+/Kaone+ e Pione-/Kaone-", 100, 0., 3);

  //   TH1F *k_star_dec = new TH1F("k_star_dec", "Decadimento di K*", 45,
  //   0.7, 1.1);
}