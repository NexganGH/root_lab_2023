#include "Particle.hpp"
#include "TROOT.h"

/// @brief Loads all files required for generation and analysis.
void LoadFiles() {
  gROOT->LoadMacro("ParticleType.cpp+");
  gROOT->LoadMacro("ResonanceType.cpp+");
  gROOT->LoadMacro("Particle.cpp+");

  gROOT->LoadMacro("generation.cpp+");

  gROOT->LoadMacro("analysis.cpp+");
}