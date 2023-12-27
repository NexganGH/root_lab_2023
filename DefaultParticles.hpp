#ifndef DEFAULT_PARTICLES_H
#define DEFAULT_PARTICLES_H

#include "Particle.hpp"

class DefaultParticles {
public:
  static void Load() {
    Particle::AddParticleType("P+", 0.13957, 1, 0);
    Particle::AddParticleType("P-", 0.13957, -1, 0);
    Particle::AddParticleType("K+", 0.49367, 1, 0);
    Particle::AddParticleType("K-", 0.49367, -1, 0);
    Particle::AddParticleType("pr+", 0.93827, 1, 0);
    Particle::AddParticleType("pr-", 0.93827, -1, 0);
    Particle::AddParticleType("K*", 0.89166, 0, 0.050);
  }
};

#endif