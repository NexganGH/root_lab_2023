#include "Particle.hpp"

#include <cmath>
#include <iostream>

int Particle::fNumParticleType = 0;
ParticleType *Particle::fParticleType[Particle::fMaxNumParticleType];

Particle::Particle(const char *particleTypeName, double fPx, double fPy,
                   double fPz)
    : fPx_{fPx}, fPy_{fPy}, fPz_{fPz} {
  fIndex = FindParticle(particleTypeName);
  if (fIndex < 0) {
    std::cout << "Particella dal nome " << particleTypeName << " non trovata!"
              << std::endl;
    fIndex = 0; // Per evitare errori dopo...
  }
}

Particle::Particle() { fIndex = -1; };

int Particle::GetIndex() const { return fIndex; }

// Ritorna -1 se non la trova.
int Particle::FindParticle(const char *name) {
  for (auto i = 0; i < fNumParticleType; i++) {
    if (fParticleType[i]->GetName() == name) {
      return i;
    }
  }

  return -1;
}

ParticleType *Particle::GetParticle(const int index) {
  return fParticleType[index];
}

int Particle::AddParticleType(const char *name, double mass, int charge,
                              double width) {
  if (fNumParticleType >= fMaxNumParticleType) {
    std::cout << "Particelle max raggiunte." << std::endl;
    return -1;
  }

  auto index = FindParticle(name);
  if (index != -1) {
    std::cout << name << " già presente." << std::endl;
    return index;
  }

  if (width == 0)
    fParticleType[fNumParticleType] = new ParticleType(name, mass, charge);
  else
    fParticleType[fNumParticleType] =
        new ResonanceType(name, mass, charge, width);

  return fNumParticleType++;
}

void Particle::ResetParticleTypes() { fNumParticleType = 0; }

int Particle::SetIndex(int index) {
  if (index >= fMaxNumParticleType)
    throw std::invalid_argument("Particella num. " + std::to_string(index) +
                                "non esiste. ");
  fIndex = index;
  return fIndex;
}

int Particle::SetIndex(const char *name) {
  auto index = FindParticle(name);
  if (index == -1)
    throw std::invalid_argument("Particella " + std::string(name) +
                                " non esiste.");
  fIndex = index;
  return index;
}

void Particle::PrintAllTypes() {
  for (auto i = 0; i < fNumParticleType; i++) {
    fParticleType[i]->Print();
  }
}

ParticleType *Particle::GetType() const { return fParticleType[fIndex]; }

void Particle::Print() const {
  std::cout << "--- Particle has name: " << GetType()->GetName() << " ---"
            << std::endl;
  std::cout << "(Px, Py, Pz) = (" << fPx_ << ", " << fPy_ << ", " << fPz_
            << "), ";

  std::cout << "m = " << GetMass() << std::endl;
}

double Particle::GetFPx() const { return fPx_; }
double Particle::GetFPy() const { return fPy_; }
double Particle::GetFPz() const { return fPz_; }

void Particle::SetP(double px, double py, double pz) {
  fPx_ = px;
  fPy_ = py;
  fPz_ = pz;
}

double Particle::GetTotalEnergy() const {
  return std::sqrt(std::pow(GetType()->GetMass(), 2) + std::pow(fPx_, 2) +
                   std::pow(fPy_, 2) + std::pow(fPz_, 2));
}

double Particle::GetInvariantMass(Particle &p2) const {
  auto totalEnergy = pow(GetTotalEnergy() + p2.GetTotalEnergy(), 2);
  auto totalP =
      pow(fPx_ + p2.fPx_, 2) + pow(fPy_ + p2.fPy_, 2) + pow(fPz_ + p2.fPz_, 2);
  // auto totalP = fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_;

  return std::sqrt(totalEnergy - totalP);
}

double Particle::GetMass() const { return GetType()->GetMass(); }

int Particle::Decay2body(Particle &dau1, Particle &dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    auto pType = fParticleType[fIndex];

    massMot += pType->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf("Decayment cannot be preformed because mass is too low in this "
           "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy =
      sqrt(fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_ + massMot * massMot);

  double bx = fPx_ / energy;
  double by = fPy_ / energy;
  double bz = fPz_ / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}
void Particle::Boost(double bx, double by, double bz) {

  double energy = GetTotalEnergy();
  // std::cout << "Gettotalenergy: " << energy << std::endl;
  //  Boost this Lorentz vector

  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  // std::cout << "b2: " << b2 << std::endl;
  double bp = bx * fPx_ + by * fPy_ + bz * fPz_;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  /// std::cout << "gamma " << gamma << ", " << gamma2 << std::endl;

  fPx_ += gamma2 * bp * bx + gamma * bx * energy;
  fPy_ += gamma2 * bp * by + gamma * by * energy;
  fPz_ += gamma2 * bp * bz + gamma * bz * energy;
}