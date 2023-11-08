#include "TROOT.h"

void ReloadGeneration() { gROOT->LoadMacro("generation.cpp"); }

void LoadFiles() {
  gROOT->LoadMacro("ParticleType.cpp+");
  gROOT->LoadMacro("ResonanceType.cpp+");
  gROOT->LoadMacro("Particle.cpp+");
  ReloadGeneration();
}

// void LoadAndStart() {

//   std::cout << "Loading..." << std::endl;
//   LoadFiles();
//   std::cout << "Loaded files. Starting generation..." << std::endl;

//   StartGeneration();

//   std::cout << "Generation completed." << std::endl;
// }