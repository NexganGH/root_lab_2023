#include "ParticleType.hpp"
#include <cassert>
#include <iostream>

ParticleType::ParticleType(const char *name, const double mass,
                           const int charge)
    : fName_{name}, fMass_{mass}, fCharge_{charge} {
  assert(mass > 0);
}

const char *ParticleType::GetName() const { return fName_; }

double ParticleType::GetMass() const { return fMass_; }

double ParticleType::GetCharge() const { return fCharge_; }

void ParticleType::Print() const {
  std::cout << "--- fName: " << fName_ << " ---" << std::endl;
  std::cout << "fMass: " << fMass_ << std::endl;
  std::cout << "fCharge: " << fCharge_ << std::endl;
}

double ParticleType::GetWidth() const { return 0; }
