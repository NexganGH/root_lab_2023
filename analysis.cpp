#include "DefaultParticles.hpp"
#include "Particle.hpp"

#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"

#include <iostream>

void PrintParticleTypes(TH1F *histo_particles) {
  std::cout << "--- Particle types" << std::endl;

  auto total_n = histo_particles->GetEntries();
  // std::cout << "Total entries: " << total_n << std::endl;
  for (int i = 0; i < histo_particles->GetNbinsX(); i++) {

    double n_particle_type = histo_particles->GetBinContent(i + 1);
    double n_particle_error = histo_particles->GetBinError(i + 1);
    auto type = Particle::GetParticleType(i);

    auto type_percentage = n_particle_type / total_n;
    auto error_percentage = n_particle_error / total_n;

    std::cout << "Particle " << type->GetName() << ": " << type_percentage
              << ", error: " << error_percentage << std::endl;
  }

  std::cout << "------" << std::endl << std::endl;
}

void PrintAngularFit(TH1D *histoAnglesPx, TH1D *histoAnglesPy) {

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
  print_single_angle(histoAnglesPx);

  std::cout << "--- Angle py --- " << std::endl;
  print_single_angle(histoAnglesPy);

  std::cout << "------" << std::endl << std::endl;
}

void PrintPModuleFit(TH1F *histoPModule) {

  std::cout << "--- P Module ---" << std::endl;
  TF1 *f = new TF1("functionPModule", "[0]*exp(-[1] * x)");
  histoPModule->Fit(f, "Q");
  std::cout << "f(x) = " << f->GetParameter(0) << " * e^(-"
            << f->GetParameter(1) << "*x)" << std::endl;
  std::cout << "Parameter errors (in order) = ";

  for (int i = 0; i < 2; i++) {
    std::cout << f->GetParError(i) << ", ";
  }
  std::cout << std::endl;
  std::cout << "Chi square/NDF = " << f->GetChisquare() / f->GetNDF()
            << std::endl;

  std::cout << "Fit probability: " << f->GetProb() << std::endl;
  std::cout << "------" << std::endl << std::endl;
}

void PrintGausFit(TH1F *histoSottr, const char *title) {
  std::cout << "--- " << title << " ---" << std::endl;

  TF1 *f = new TF1("fitGaus", "gaus");
  // f->SetParameters(1, 0.89, 0.05);
  histoSottr->Fit(f, "Q");

  std::cout << "Amplitude K* = " << f->GetParameter(0) << std::endl;
  std::cout << "Mass K* (mean) = " << f->GetParameter(1) << std::endl;
  std::cout << "Width K* (std) = " << f->GetParameter(2) << std::endl;
  std::cout << "Amplitude, Mass and Width errors (in order) = ";

  for (int i = 0; i < 3; i++) {
    std::cout << f->GetParError(i) << ", ";
  }
  std::cout << std::endl;
  std::cout << "Chi square/NDF = " << f->GetChisquare() / f->GetNDF()
            << std::endl;

  std::cout << "Fit probability: " << f->GetProb() << std::endl;
  std::cout << "------" << std::endl << std::endl;
}

/// @brief Subtracts two histograms
/// @param disc_histo
/// @param conc_histo
/// @return
TH1F *SubtractInvMass(TH1F *disc_histo, TH1F *conc_histo) {
  TH1F *res = new TH1F(*disc_histo);
  res->Add(conc_histo, -1);
  res->SetEntries(disc_histo->GetEntries() - conc_histo->GetEntries());
  res->SetXTitle("Massa invariante (GeV/c^2)");
  res->SetYTitle("Entries");
  res->GetXaxis()->SetRangeUser(0, 2); // zooming in to see the gaussian

  return res;
}

void Analysis() {
  DefaultParticles::Load();

  gStyle->SetOptFit(1111);

  Int_t cIndex = 5000;
  TColor *darkGreen = new TColor(cIndex, 0, 102.0 / 255, 0);
  gStyle->SetHistLineColor(cIndex);

  TFile *file = new TFile("save.root");

  auto *histoParticleTypes = file->Get<TH1F>("histoParticleTypes");
  histoParticleTypes->SetXTitle("Tipo particella");
  auto *histoPModule = file->Get<TH1F>("histoPModule");
  histoPModule->SetYTitle("Entries");
  histoPModule->SetXTitle("Modulo del momento (GeV/c)");
  auto *histoAngles = file->Get<TH2F>("histoAngles");
  auto *histoAnglesPx = file->Get<TH1D>("histoAngles_px");
  auto *histoAnglesPy = file->Get<TH1D>("histoAngles_py");
  auto *histoTransverseImpulse = file->Get<TH1F>("histoTransverseImpulse");
  auto *histoEnergy = file->Get<TH1F>("histoEnergy");
  auto *histoInvariantMassGeneral =
      file->Get<TH1F>("histoInvariantMassGeneral");
  auto *histoDiscInvariantMass = file->Get<TH1F>("histoDiscInvariantMass");
  auto *histoConcInvariantMass = file->Get<TH1F>("histoConcInvariantMass");
  auto *histoDiscordantPK = file->Get<TH1F>("histoDiscordantPK");
  auto *histoConcordantPK = file->Get<TH1F>("histoConcordantPK");
  auto *histoKDecInvariantMass = file->Get<TH1F>("histoKDecInvariantMass");

  PrintParticleTypes(histoParticleTypes);
  PrintAngularFit(histoAnglesPx, histoAnglesPy);
  PrintPModuleFit(histoPModule);

  // CANVAS 1

  TCanvas *canvas1 = new TCanvas("canvas1", "Particelle");
  canvas1->Divide(2, 2);

  canvas1->cd(1);
  histoParticleTypes->DrawCopy();

  canvas1->cd(2);
  histoPModule->DrawCopy();

  canvas1->cd(3);
  histoTransverseImpulse->DrawCopy();

  canvas1->cd(4);
  histoEnergy->DrawCopy();

  // CANVAS 2

  auto canvas2 = new TCanvas("canvas2", "Angoli");
  canvas2->Divide(2, 2);

  canvas2->cd(1);
  histoAngles->DrawCopy("LEGO");

  canvas2->cd(2);
  histoAnglesPx->DrawCopy();

  canvas2->cd(3);
  histoAnglesPy->DrawCopy();

  canvas2->cd(4);
  histoParticleTypes->DrawCopy();

  // CANVAS 3

  auto *canvas3 = new TCanvas("canvas3", "Massa invariante");
  canvas3->Divide(2, 3);

  canvas3->cd(1);
  histoInvariantMassGeneral->DrawCopy();

  canvas3->cd(2);
  histoDiscInvariantMass->DrawCopy();

  canvas3->cd(3);
  histoConcInvariantMass->DrawCopy();

  canvas3->cd(4);
  histoDiscordantPK->DrawCopy();

  canvas3->cd(5);
  histoConcordantPK->DrawCopy();

  canvas3->cd(6);
  histoKDecInvariantMass->DrawCopy();

  // CANVAS 4

  auto *canvas4 = new TCanvas("canvas4", "Sottrazioni massa invariante");
  canvas4->Divide(2, 2);

  auto histoInvMassDiff =
      SubtractInvMass(histoDiscInvariantMass, histoConcInvariantMass);
  histoInvMassDiff->SetTitle(
      "Differenza massa invariante fra tutte le particelle");
  canvas4->cd(1);
  histoInvMassDiff->DrawCopy("P");
  PrintGausFit(histoInvMassDiff, "General invariant mass subtraction");
  histoInvMassDiff->DrawCopy("P");

  auto histoPKInvMassDiff =
      SubtractInvMass(histoDiscordantPK, histoConcordantPK);
  histoPKInvMassDiff->SetTitle("Differenza massa invariante tra pioni e kaoni");
  canvas4->cd(2);
  histoPKInvMassDiff->DrawCopy("P");
  PrintGausFit(histoPKInvMassDiff, "P&K invariant mass subtraction");
  histoPKInvMassDiff->DrawCopy("P");

  canvas4->cd(3);
  histoKDecInvariantMass->DrawCopy();
  PrintGausFit(histoKDecInvariantMass, "K* invariant mass");
  histoKDecInvariantMass->DrawCopy("P");

  // CANVAS 5
  auto *canvas5 = new TCanvas("canvas5", "Fit");
  canvas5->Divide(2, 2);

  canvas5->cd(1);
  histoParticleTypes->DrawCopy("HISTO");
  canvas5->cd(2);
  histoPModule->DrawCopy();
  canvas5->cd(3);
  histoAnglesPx->DrawCopy();
  canvas5->cd(4);
  histoAnglesPy->DrawCopy();

  file->Close();
}