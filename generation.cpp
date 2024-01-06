#include "DefaultParticles.hpp"

#include "TCanvas.h"
#include "TColor.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TRandom.h"
#include "TStyle.h"

#include <array>
#include <cmath>
#include <iostream>

const int N_EVENTS = 1E3;
const int N_PARTICLES_PER_EVENT = 100;

/// @brief Checks if one particle is pione and the other kaone.
/// @param first
/// @param second
/// @return
bool IsPioneKaone(Particle &first, Particle &second) {
  bool foundP, foundK;

  auto iType = first.GetType()->GetName();
  auto jType = second.GetType()->GetName();

  if (strcmp(iType, "P+") == 0 || strcmp(iType, "P-") == 0) {
    if (strcmp(jType, "K+") == 0 || strcmp(jType, "K-") == 0) {
      return true;
    }
  } else if (strcmp(jType, "P+") == 0 || strcmp(jType, "P-") == 0) {
    if (strcmp(iType, "K+") == 0 || strcmp(iType, "K-") == 0) {
      return true;
    }
  }
  return false;
}

/// @brief Sets the options for this histogram.
/// @param histo
/// @param xTitle Title of the x axis.
void SetHistoOptions(TH1F *histo, const char *xTitle) {
  histo->SetXTitle(xTitle);
  histo->SetYTitle("Entries");
}

/// @brief Starts the geneneration.
void StartGeneration() {
  TFile *file = new TFile("save.root", "RECREATE");
  std::cout << "Starting generation..." << std::endl;
  DefaultParticles::Load();

  gRandom->SetSeed();
  gStyle->SetOptFit(1111);

  Int_t cIndex = 5000;
  TColor *darkGreen = new TColor(cIndex, 0, 102.0 / 255, 0);
  gStyle->SetHistLineColor(cIndex);

  TH1F *histoParticleTypes =
      new TH1F("histoParticleTypes", "Particle Types", 7, 0, 7);
  histoParticleTypes->Sumw2();
  TH1F *histoPModule = new TH1F("histoPModule", "P Module", 100, 0., 5);
  histoPModule->Sumw2();
  TH2F *histoAngles =
      new TH2F("histoAngles", "Theta & Phi", 100, 0, 2 * M_PI, 100, 0, M_PI);
  histoAngles->Sumw2();
  TH1F *histoTransverseImpulse =
      new TH1F("histoTransverseImpulse", "Impulso traverso", 100, 0., 1);
  histoTransverseImpulse->Sumw2();
  TH1F *histoEnergy = new TH1F("histoEnergy", "Energia", 100, 0., 3);
  histoEnergy->Sumw2();
  TH1F *histoInvariantMassGeneral =
      new TH1F("histoInvariantMassGeneral", "Massa invariante", 100, 0., 3);
  histoInvariantMassGeneral->Sumw2();
  TH1F *histoDiscInvariantMass = new TH1F(
      "histoDiscInvariantMass", "Massa invariante discorde", 100, 0., 3);
  histoDiscInvariantMass->Sumw2();
  TH1F *histoConcInvariantMass = new TH1F(
      "histoConcInvariantMass", "Massa invariante concorde", 100, 0., 3);
  histoConcInvariantMass->Sumw2();
  TH1F *histoDiscordantPK = new TH1F(
      "histoDiscordantPK", "Pione+/Kaone- e Pione-/Kaone+", 100, 0., 3);
  histoDiscordantPK->Sumw2();
  TH1F *histoConcordantPK = new TH1F(
      "histoConcordantPK", "Pione+/Kaone+ e Pione-/Kaone-", 100, 0., 3);
  histoConcordantPK->Sumw2();
  TH1F *histoKDecInvariantMass =
      new TH1F("histoKDecInvariantMass", "Decadimento di K*", 45, 0.7, 1.1);
  histoKDecInvariantMass->Sumw2();

  Particle eventParticles[N_PARTICLES_PER_EVENT + 30];

  for (auto event = 0; event < N_EVENTS; event++) {
    int decayIndex = N_PARTICLES_PER_EVENT;
    for (auto particleIndex = 0; particleIndex < N_PARTICLES_PER_EVENT;
         particleIndex++) {
      auto phi = gRandom->Uniform(2 * M_PI);
      auto theta = gRandom->Uniform(M_PI);
      auto momentum = gRandom->Exp(1); // 1 GeV

      histoAngles->Fill(phi, theta);
      histoPModule->Fill(momentum);

      auto pX = momentum * std::sin(theta) * cos(phi);
      auto pY = momentum * std::sin(theta) * sin(phi);
      auto pZ = momentum * std::cos(theta);

      auto mom_trasv = std::sqrt(std::pow(pX, 2) + std::pow(pY, 2));
      histoTransverseImpulse->Fill(mom_trasv);

      auto partType = DefaultParticles::GenerateParticleType();
      Particle part(partType, pX, pY, pZ);

      auto p = part.GetIndex();

      histoParticleTypes->Fill(p);

      auto totalEnergy = part.GetTotalEnergy();

      histoEnergy->Fill(totalEnergy);
      eventParticles[particleIndex] = part;

      if (strcmp(part.GetType()->GetName(), "K*") == 0) {
        Particle dec1;
        Particle dec2;

        if (gRandom->Uniform() < 0.5) {
          dec1.SetIndex("P+");
          dec2.SetIndex("K-");
        } else {
          dec1.SetIndex("P-");
          dec2.SetIndex("K+");
        }

        auto err = part.Decay2body(dec1, dec2);
        if (err == 0) {

          histoKDecInvariantMass->Fill(dec1.GetInvariantMass(dec2));
          eventParticles[decayIndex++] = dec1;
          eventParticles[decayIndex++] = dec2;
        }
      }
    }

    for (auto i = 0; i < decayIndex; i++) {
      for (auto j = 0; j < i; j++) {
        auto first = eventParticles[i];
        auto second = eventParticles[j];

        auto sign =
            first.GetType()->GetCharge() * second.GetType()->GetCharge();

        auto invariantMass = first.GetInvariantMass(second);

        histoInvariantMassGeneral->Fill(invariantMass);

        if (sign < 0)
          histoDiscInvariantMass->Fill(invariantMass);
        else if (sign > 0)
          histoConcInvariantMass->Fill(invariantMass);
        else // sign is 0
          continue;

        if (IsPioneKaone(first, second)) {
          if (sign < 0)
            histoDiscordantPK->Fill(invariantMass);
          else if (sign > 0)
            histoConcordantPK->Fill(invariantMass);
        }
      }
    }
  }

  auto totalEntries = histoParticleTypes->GetEntries();

  TCanvas *canvas1 = new TCanvas("canvas1", "Dati Particelle");
  canvas1->Divide(2, 2);

  canvas1->cd(1);
  SetHistoOptions(histoParticleTypes, "Tipo particella");
  histoParticleTypes->DrawCopy("E");

  canvas1->cd(2);
  SetHistoOptions(histoParticleTypes, "Modulo impulso P (GeV)");
  histoPModule->DrawCopy();

  canvas1->cd(3);
  SetHistoOptions(histoTransverseImpulse, "Modulo impulso trasverso (GeV)");
  histoTransverseImpulse->DrawCopy();

  canvas1->cd(4);
  SetHistoOptions(histoEnergy, "Energia (GeV)");
  histoEnergy->DrawCopy();

  auto *canvas2 = new TCanvas("canvas2", "Angoli");
  canvas2->Divide(2, 2);
  canvas2->cd(1);

  // SetHistoOptions(histoAngles, "Angoli ");
  histoAngles->DrawCopy("LEGO");

  auto histoAngleX = histoAngles->ProjectionX();
  histoAngleX->SetTitle("Phi");
  histoAngleX->SetXTitle("Phi (rad)");
  // SetHistoOptions(histoAngleX, "Phi (rad)");
  canvas2->cd(2);
  histoAngleX->DrawCopy();

  auto histoAngleY = histoAngles->ProjectionY();
  histoAngleY->SetTitle("Theta");
  histoAngleY->SetXTitle("Theta (rad)");
  canvas2->cd(3);
  histoAngleY->DrawCopy();
  // proiezione

  auto *canvas3 = new TCanvas("canvas3", "Masse invarianti");
  canvas3->Divide(2, 3);

  canvas3->cd(1);
  SetHistoOptions(histoInvariantMassGeneral, "Massa invariante (GeV/c^2)");
  histoInvariantMassGeneral->DrawCopy();

  canvas3->cd(2);
  SetHistoOptions(histoDiscInvariantMass, "Massa invariante (GeV/c^2)");
  histoDiscInvariantMass->DrawCopy();

  canvas3->cd(3);
  SetHistoOptions(histoConcInvariantMass, "Massa invariante (GeV/c^2)");
  histoConcInvariantMass->DrawCopy();

  canvas3->cd(4);
  SetHistoOptions(histoDiscordantPK, "Massa invariante (GeV/c^2)");

  histoDiscordantPK->DrawCopy();

  canvas3->cd(5);
  SetHistoOptions(histoConcordantPK, "Massa invariante (GeV/c^2)");
  histoConcordantPK->DrawCopy();

  canvas3->cd(6);
  SetHistoOptions(histoKDecInvariantMass, "Massa invariante (GeV/c^2)");
  histoKDecInvariantMass->DrawCopy();

  file->Write();
  file->Close();

  std::cout << "Generation completed. Saved in file \"save.root\"."
            << std::endl;
}
