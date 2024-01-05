#ifndef PARTICLE_H
#define PARTICLE_H

#include "ResonanceType.hpp"

class Particle {
public:
  Particle(const char *particleTypeName, double fPx, double fPy, double fPz);
  Particle();

  int GetIndex() const;

  int SetIndex(const char *name);
  int SetIndex(int index);
  void Print() const;

  ParticleType *GetType() const;

  double GetFPx() const;
  double GetFPy() const;
  double GetFPz() const;

  void SetP(double px, double py, double pz);

  double GetTotalEnergy() const;
  double GetInvariantMass(Particle &p2) const;

  int Decay2body(Particle &dau1, Particle &dau2) const;

  double GetMass() const;

  static int AddParticleType(const char *name, double mass, int charge,
                             double width = 0);
  static void ResetParticleTypes();
  static void PrintAllTypes();
  static ParticleType *GetParticle(const int index);

private:
  const static int fMaxNumParticleType = 10;
  static ParticleType *fParticleType[fMaxNumParticleType];
  static int fNumParticleType;
  int fIndex;

  double fPx_;
  double fPy_;
  double fPz_;

  static int FindParticle(const char *name);

  void Boost(double bx, double by, double bz);
};

#endif