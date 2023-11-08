#ifndef PARTICLE_H
#define PARTICLE_H

#include "ParticleType.hpp"

class Particle {
public:
  Particle(const char *particleTypeName, double fPx, double fPy, double fPz);
  Particle();

  int getFIndex() const;

  int setIndex(const char *name);
  int setIndex(int index);
  void print() const;

  ParticleType *getType() const;

  double getFPx() const;
  double getFPy() const;
  double getFPz() const;

  void setP(double px, double py, double pz);

  double getTotalEnergy() const;
  double getInvariantMass(Particle &p2) const;

  int Decay2body(Particle &dau1, Particle &dau2) const;

  double GetMass() const;

  static int AddParticleType(const char *name, double mass, int charge,
                             double width = 0);
  static void printAllTypes();

private:
  const static int fMaxNumParticleType = 10;
  static ParticleType *fParticleType[fMaxNumParticleType];
  static int fNumParticleType;
  int fIndex;

  double fPx_;
  double fPy_;
  double fPz_;

  static int findParticle(const char *name);
  void Boost(double bx, double by, double bz);
};

#endif