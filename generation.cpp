#include "DefaultParticles.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

#include "TRandom.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

const int N_EVENTS = 1E3;
const int N_PARTICLES_PER_EVENT = 100;

const char *GenerateParticleType() {
  auto x = gRandom->Rndm();

  if (x < 0.40)
    return "P+";
  else if (x < 0.8)
    return "P-";
  else if (x < 0.85)
    return "K+";
  else if (x < 0.90)
    return "K-";
  else if (x < 0.945)
    return "pr+";
  else if (x < 0.99)
    return "pr-";
  else
    return "K*";
}

void StartGeneration() {
  TFile *file = new TFile("save.root", "RECREATE");
  std::cout << "Starting generation." << std::endl;
  DefaultParticles::Load();

  gRandom->SetSeed();

  TH1F *histo_particle_types =
      new TH1F("histo_particelle", "Particelle", 7, 0, 7);
  histo_particle_types->Sumw2();
  TH1F *histo_P_module = new TH1F("histo_P_module", "P_module", 100, 0., 1);
  histo_P_module->Sumw2();
  TH2F *histo_angle = new TH2F("histo_phi_theta", "Phi & Theta plot", 100, 0,
                               2 * M_PI, 100, 0, M_PI);
  histo_angle->Sumw2();
  TH1F *histo_impulso_trasverso =
      new TH1F("histo_impulso_trasverso", "impulso_trasverso", 100, 0., 1);
  histo_impulso_trasverso->Sumw2();
  TH1F *histo_energia = new TH1F("histo_energia", "Energia", 100, 0., 3);
  histo_energia->Sumw2();
  TH1F *inv_mass = new TH1F("inv_mass", "Invariant mass", 100, 0., 3);
  inv_mass->Sumw2();
  TH1F *disc_inv_mass =
      new TH1F("disc_inv_mass", "Discordant invariant mass", 100, 0., 3);
  disc_inv_mass->Sumw2();
  TH1F *conc_inv_mass =
      new TH1F("conc_inv_mass", "Concordant invariant mass", 100, 0., 3);
  conc_inv_mass->Sumw2();
  TH1F *p_pos_k_neg =
      new TH1F("p_pos_k_neg", "Pione+/Kaone- e Pione-/Kaone+", 100, 0., 3);
  p_pos_k_neg->Sumw2();
  TH1F *p_pos_k_pos =
      new TH1F("p_pos_k_pos", "Pione+/Kaone+ e Pione-/Kaone-", 100, 0., 3);
  p_pos_k_pos->Sumw2();
  TH1F *k_star_dec = new TH1F("k_star_dec", "Decadimento di K*", 45, 0.7, 1.1);
  k_star_dec->Sumw2();

  // std::array<Particle, N_PARTICLES_PER_EVENT + 30> eventParticles;
  Particle eventParticles[N_PARTICLES_PER_EVENT + 30];

  for (auto event = 0; event < N_EVENTS; event++) {
    // Particle *eventParticles[N_PARTICLES_PER_EVENT + 20];
    int decayIndex = N_PARTICLES_PER_EVENT;
    for (auto particleIndex = 0; particleIndex < N_PARTICLES_PER_EVENT;
         particleIndex++) {
      // Particle part; // = new Particle();
      double phi = gRandom->Uniform(2 * M_PI);
      double theta = gRandom->Uniform(M_PI);
      double momentum = gRandom->Exp(1); // 1 GeV

      histo_angle->Fill(phi, theta);
      histo_P_module->Fill(momentum);

      auto pX = momentum * std::sin(phi) * cos(theta);
      auto pY = momentum * std::sin(phi) * sin(theta);
      auto pZ = momentum * std::cos(phi);

      auto mom_trasv = std::sqrt(std::pow(pX, 2) + std::pow(pY, 2));
      histo_impulso_trasverso->Fill(mom_trasv);

      auto partType = GenerateParticleType();
      Particle part(partType, pX, pY, pZ);

      auto p = part.getFIndex();

      histo_particle_types->Fill(p);

      auto total_energy = part.getTotalEnergy();

      // std::cout << "FILL ENERGY " << std::endl;
      histo_energia->Fill(total_energy);
      eventParticles[particleIndex] = part;

      if (strcmp(part.getType()->getFName(), "K*") == 0) {
        Particle dec1;
        Particle dec2;

        if (gRandom->Uniform() < 0.5) {
          dec1.setIndex("P+");
          dec2.setIndex("K-");
        } else {
          dec1.setIndex("P-");
          dec2.setIndex("K+");
        }

        auto err = part.Decay2body(dec1, dec2);
        if (err == 0) {

          k_star_dec->Fill(dec1.getInvariantMass(dec2));
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
            first.getType()->getFCharge() * second.getType()->getFCharge();

        auto invariantMass = first.getInvariantMass(second);

        inv_mass->Fill(invariantMass);

        if (sign < 0)
          disc_inv_mass->Fill(invariantMass);
        else if (sign > 0) // if (i < N_PARTICLES_PER_EVENT) (per escludere le
                           // particelle figlie)
          conc_inv_mass->Fill(invariantMass);
        else // sign is 0
          continue;

        int pIndex = -1;
        int kIndex = -1;

        auto iType = first.getType()->getFName();
        auto jType = second.getType()->getFName();

        if (strcmp(iType, "P+") == 0 || strcmp(iType, "P-") == 0) {
          pIndex = i;
          if (strcmp(jType, "K+") == 0 || strcmp(jType, "K-") == 0) {
            kIndex = i;
          }
        } else if (strcmp(jType, "P+") == 0 || strcmp(jType, "P-") == 0) {
          pIndex = i;
          if (strcmp(iType, "K+") == 0 || strcmp(iType, "K-") == 0) {
            kIndex = i;
          }
        }

        // In this case we know we have a P and a K
        if (pIndex != -1 && kIndex != -1) {
          auto sign =
              first.getType()->getFCharge() * second.getType()->getFCharge();
          if (sign < 0)
            p_pos_k_neg->Fill(invariantMass);
          else
            p_pos_k_pos->Fill(invariantMass);
        }
      }
    }
  }

  double total_entries = histo_particle_types->GetEntries();

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

  auto *canvas2 = new TCanvas();
  canvas2->Divide(2, 2);

  canvas2->cd(1);
  histo_angle->DrawCopy();

  auto histo_angle_px = histo_angle->ProjectionX();
  canvas2->cd(2);
  histo_angle_px->DrawCopy();

  auto histo_angle_py = histo_angle->ProjectionY();
  canvas2->cd(3);
  histo_angle_py->DrawCopy();
  // proiezione

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

  file->Write();

  file->Close();
}
