#include "ParticleType.hpp"
#include <cassert>
#include <iostream>

ParticleType::ParticleType(const char *name, double mass, int charge)
    : fName_{name}, fMass_{mass}, fCharge_{charge} {
  assert(mass > 0);
}

const char *ParticleType::getFName() const { return fName_; }

double ParticleType::getFMass() const { return fMass_; }

double ParticleType::getFCharge() const { return fCharge_; }

void ParticleType::print() const {
  std::cout << "--- fName: " << fName_ << " ---" << std::endl;
  std::cout << "fMass: " << fMass_ << std::endl;
  std::cout << "fCharge: " << fCharge_ << std::endl;
}

double ParticleType::GetWidth() const { return 0; }
