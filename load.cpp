#include "Particle.hpp"
#include "TROOT.h"

void ReloadGeneration() { gROOT->LoadMacro("generation.cpp"); }

void LoadFiles() {
  gROOT->LoadMacro("ParticleType.cpp+");
  gROOT->LoadMacro("ResonanceType.cpp+");
  gROOT->LoadMacro("Particle.cpp+");

  ReloadGeneration();
}