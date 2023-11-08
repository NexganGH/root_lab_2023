#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

#include "TRandom.h"
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
  std::cout << "Starting generation." << std::endl;

  Particle::AddParticleType("P+", 0.13957, 1, 0);
  Particle::AddParticleType("P-", 0.13957, -1, 0);
  Particle::AddParticleType("K+", 0.49367, 1, 0);
  Particle::AddParticleType("K-", 0.49367, -1, 0);
  Particle::AddParticleType("pr+", 0.93827, 1, 0);
  Particle::AddParticleType("pr-", 0.93827, -1, 0); // protoni negativi?
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);

  gRandom->SetSeed();

  TH1F *histo_P_module =
      new TH1F("histo_P_module", "P_module", 100, 0.,
               1E-9); // ho scelto il numero dei bin a caso e il limite
                      // superiore a caso perchè dovrebbe essere genera da
                      // distribuzioni eponenziali
  // TH1F *histo_phi = new TH1F("histo_phi", "phi", 100, 0., 2 * M_PI);
  // TH1F *histo_theta = new TH1F("histo_theta", "theta", 100, 0., M_PI);
  TH2F *histo_angle = new TH2F("histo_phi_theta", "Phi & Theta plot", 100, 0,
                               2 * M_PI, 100, 0, M_PI);
  TH1F *histo_impulso_trasverso =
      new TH1F("histo_impulso_trasverso", "impulso_trasverso", 100, 0., 1E-9);
  TH1F *histo_energia = new TH1F("histo_energia", "energia", 100, 0., 1.E-2);

  // TH1F *hiso_theta = new TH1F("histo_theta", "theta", 100, 0., M_PI);
  // TH1F *hiso_theta = new TH1F("histo_theta", "theta", 100, 0., M_PI);

  TH1F *disc_inv_mass =
      new TH1F("disc_inv_mass", "Discordant invariant mass", 100, 0., 1.);
  TH1F *conc_inv_mass =
      new TH1F("conc_inv_mass", "Concordant invariant mass", 100, 0., 1.);

  std::vector<Particle> eventParticles;
  for (auto event = 0; event < N_EVENTS; event++) {
    // Particle *eventParticles[N_PARTICLES_PER_EVENT + 20];

    for (auto particle = 0; particle < N_PARTICLES_PER_EVENT; particle++) {
      Particle part; // = new Particle();
      double phi = gRandom->Uniform(2 * M_PI);
      double theta = gRandom->Uniform(M_PI);
      double momentum = gRandom->Exp(1E9 * 1.602E-19); // 1 GeV

      histo_angle->Fill(phi, theta);
      histo_P_module->Fill(momentum);

      auto pX = momentum * std::sin(phi) * cos(theta);
      auto pY = momentum * std::sin(phi) * sin(theta);
      auto pZ = momentum * std::cos(phi);

      auto mom_trasv = std::sqrt(std::pow(pX, 2) + std::pow(pY, 2));
      histo_impulso_trasverso->Fill(mom_trasv);

      part.setP(pX, pY, pZ);

      part.setIndex(GenerateParticleType());

      histo_energia->Fill(part.getTotalEnergy());

      eventParticles.push_back(part);

      if (strcmp(part.getType()->getFName(), "K*") == 0) {
        Particle dec1; // = new Particle();
        Particle dec2; // = new Particle();

        if (gRandom->Uniform() < 0.5) {
          dec1.setIndex("P+");
          dec2.setIndex("K-");
        } else {
          dec1.setIndex("P-");
          dec2.setIndex("K+");
        }

        auto x = part.Decay2body(dec1, dec2);
        if (x == 0) {
          eventParticles.push_back(dec1);
          eventParticles.push_back(dec2);
        }
      }
    }

    for (auto i = 0; i < eventParticles.size(); i++) {
      for (auto j = i; j < eventParticles.size(); j++) {
        auto sign = eventParticles[i].getType()->getFCharge() *
                    eventParticles[j].getType()->getFCharge();

        if (sign < 0)
          disc_inv_mass->Fill(
              eventParticles[i].getInvariantMass(eventParticles[j]));
        else
          conc_inv_mass->Fill(
              eventParticles[i].getInvariantMass(eventParticles[j]));
      }
    }

    eventParticles.clear();
    eventParticles.shrink_to_fit();
  }

  TCanvas *canvas = new TCanvas();

  canvas->Divide(2, 3);

  canvas->cd(1);
  histo_P_module->Draw();

  canvas->cd(2);
  histo_angle->Draw();

  canvas->cd(3);
  histo_impulso_trasverso->Draw();

  canvas->cd(4);
  histo_energia->Draw();

  canvas->cd(5);
  disc_inv_mass->Draw();

  canvas->cd(6);
  conc_inv_mass->Draw();
}
