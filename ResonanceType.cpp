#include "ResonanceType.hpp"
#include <cassert>
#include <iostream>

ResonanceType::ResonanceType(const char *name, double mass, int charge,
                             double width)
    : ParticleType(name, mass, charge), fWidth_{width} {
  assert(width > 0);
}

double ResonanceType::GetWidth() const { return fWidth_; }

void ResonanceType::print() const {

  std::cout << "--- " << getFName() << " ---" << std::endl;
  std::cout << "fMass: " << getFMass() << std::endl;
  std::cout << "fCharge: " << getFCharge() << std::endl;
  std::cout << "fWidth: " << GetWidth() << std::endl;
}
