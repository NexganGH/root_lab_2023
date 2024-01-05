#include "ResonanceType.hpp"
#include <cassert>
#include <iostream>

ResonanceType::ResonanceType(const char *name, double mass, int charge,
                             double width)
    : ParticleType(name, mass, charge), fWidth_{width} {
  assert(width > 0);
}

double ResonanceType::GetWidth() const { return fWidth_; }

void ResonanceType::Print() const {

  std::cout << "--- " << GetName() << " ---" << std::endl;
  std::cout << "fMass: " << GetMass() << std::endl;
  std::cout << "fCharge: " << GetCharge() << std::endl;
  std::cout << "fWidth: " << GetWidth() << std::endl;
}
