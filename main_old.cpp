#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include <iostream>

int main() {
  Particle::AddParticleType("part1", 10, 5, 10);
  Particle::AddParticleType("part2", 5, 10);

  Particle part1("part1", 10, 3, 4);

  Particle part2("part1", 5, 10, 2);

  part2.setIndex("part2");
  // part1.setIndex("teasdawd");

  part1.print();
  Particle::printAllTypes();

  //   ParticleType particle{"particle1", 10, 1};
  //   ResonanceType resonance{"resonance1", 10, -1, 10};

  //   particle.print();
  //   resonance.print();

  //   ParticleType *particles[] = {new ParticleType("particle2", 10, 1),
  //                                new ResonanceType("resonance2", 5, -1, 50)};

  //   for (auto i = 0; i < 2; i++)
  //     particles[i]->print();

  //   for (auto i = 0; i < 2; i++)
  //     delete particles[i];
}