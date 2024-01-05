#ifndef DEFAULT_PARTICLES_H
#define DEFAULT_PARTICLES_H

#include "Particle.hpp"
#include "TRandom.h"

class DefaultParticles {
public:
  static void Load() {
    Particle::ResetParticleTypes();

    Particle::AddParticleType("P+", 0.13957, 1, 0);
    Particle::AddParticleType("P-", 0.13957, -1, 0);
    Particle::AddParticleType("K+", 0.49367, 1, 0);
    Particle::AddParticleType("K-", 0.49367, -1, 0);
    Particle::AddParticleType("pr+", 0.93827, 1, 0);
    Particle::AddParticleType("pr-", 0.93827, -1, 0);
    Particle::AddParticleType("K*", 0.89166, 0, 0.050);
  }

  static const char *GenerateParticleType() {
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
};

#endif