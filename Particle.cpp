#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include <cmath>
#include <iostream>

int Particle::fNumParticleType = 0;
ParticleType *Particle::fParticleType[Particle::fMaxNumParticleType];

Particle::Particle(const char *particleTypeName, double fPx, double fPy,
                   double fPz)
    : fPx_{fPx}, fPy_{fPy}, fPz_{fPz} {
  fIndex = findParticle(particleTypeName);
  if (fIndex < 0) {
    std::cout << "Particella dal nome " << particleTypeName << " non trovata!"
              << std::endl;
    fIndex = 0; // Per evitare errori dopo...
  }
}

Particle::Particle() { fIndex = -1; };

int Particle::getFIndex() const { return fIndex; }

// Ritorna -1 se non la trova.
int Particle::findParticle(const char *name) {
  for (auto i = 0; i < fNumParticleType; i++) {
    if (fParticleType[i]->getFName() == name) {
      return i;
    }
  }

  return -1;
}

int Particle::AddParticleType(const char *name, double mass, int charge,
                              double width) {
  if (fNumParticleType >= fMaxNumParticleType) {
    std::cout << "Particelle max raggiunte." << std::endl;
    return -1;
  }

  auto index = findParticle(name);
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

int Particle::setIndex(int index) {
  if (index >= fMaxNumParticleType)
    throw std::invalid_argument("Particella num. " + std::to_string(index) +
                                "non esiste. ");
  fIndex = index;
  return fIndex;
}

int Particle::setIndex(const char *name) {
  auto index = findParticle(name);
  if (index == -1)
    throw std::invalid_argument("Particella " + std::string(name) +
                                " non esiste.");
  fIndex = index;
  return index;
}

void Particle::printAllTypes() {
  for (auto i = 0; i < fNumParticleType; i++) {
    fParticleType[i]->print();
  }
}

ParticleType *Particle::getType() const { return fParticleType[fIndex]; }

void Particle::print() const {
  std::cout << "--- Particle has name: " << getType()->getFName() << " ---"
            << std::endl;
  std::cout << "(Px, Py, Pz) = (" << fPx_ << ", " << fPy_ << ", " << fPz_
            << "), ";

  std::cout << "m = " << GetMass() << std::endl;
}

double Particle::getFPx() const { return fPx_; }
double Particle::getFPy() const { return fPy_; }
double Particle::getFPz() const { return fPz_; }

void Particle::setP(double px, double py, double pz) {
  fPx_ = px;
  fPy_ = py;
  fPz_ = pz;
}

double Particle::getTotalEnergy() const {
  return sqrt(std::pow(getType()->getFMass(), 2) + pow(fPx_, 2) + pow(fPy_, 2) +
              pow(fPz_, 2));
}

double Particle::getInvariantMass(Particle &p2) const {
  auto totalEnergy = pow(getTotalEnergy() + p2.getTotalEnergy(), 2);
  auto totalP =
      pow(fPx_ + p2.fPx_, 2) + pow(fPy_ + p2.fPy_, 2) + pow(fPz_ + p2.fPz_, 2);
  // auto totalP = fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_;

  return std::sqrt(totalEnergy - totalP);
}

double Particle::GetMass() const { return getType()->getFMass(); }

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

  // std::cout << "pout: " << pout << std::endl;
  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.setP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.setP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  // std::cout << "phi: " << phi << std::endl;
  // std::cout << "theta: " << theta << std::endl;

  double energy =
      sqrt(fPx_ * fPx_ + fPy_ * fPy_ + fPz_ * fPz_ + massMot * massMot);

  // std::cout << "energy: " << energy << std::endl;

  double bx = fPx_ / energy;
  double by = fPy_ / energy;
  double bz = fPz_ / energy;

  // std::cout << "(bx, by, bz) = " << bx << ", " << by << ", " << bz <<
  // std::endl;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}
void Particle::Boost(double bx, double by, double bz) {

  double energy = getTotalEnergy();
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