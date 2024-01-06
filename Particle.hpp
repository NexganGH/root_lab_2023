#ifndef PARTICLE_H
#define PARTICLE_H

#include "ResonanceType.hpp"

class Particle {
public:
  Particle(const char *particleTypeName, double fPx, double fPy, double fPz);
  Particle();
  /// @brief Gets the index of the particle type.
  /// @return
  int GetIndex() const;

  /// @brief Sets the particle type by providing its name (ex: K+, P+, P-,
  /// etc...)
  /// @param name
  /// @return
  int SetIndex(const char *name);

  /// @brief Sets the particle type by providing its id.
  /// @param index The id of the particle type (ex: 0, 1, 5, etc...)
  /// @return
  int SetIndex(int index);

  /// @brief Prints information about this particle.
  void Print() const;

  /// @brief Returns the ParticleType this particle is.
  /// @return
  ParticleType *GetType() const;

  /// @brief Returns the x component of the impulse.
  /// @return
  double GetPx() const;
  /// @brief Returns the y component of the impulse.
  /// @return
  double GetPy() const;
  /// @brief Returns the z component of the impulse.
  /// @return
  double GetPz() const;

  /// @brief Sets the components of the impulse.
  /// @param px
  /// @param py
  /// @param pz
  void SetP(double px, double py, double pz);

  /// @brief Calculates the total energy with c=1.
  /// @ref https://en.wikipedia.org/wiki/Energy%E2%80%93momentum_relation
  /// @return
  double GetTotalEnergy() const;

  /// @brief Calculates the invariant mass.
  /// @param p2
  /// @ref https://en.wikipedia.org/wiki/Invariant_mass
  /// @return
  double GetInvariantMass(Particle &p2) const;

  /// @brief Simulates the decay of two bodies.
  /// @param dau1
  /// @param dau2
  /// @return
  int Decay2body(Particle &dau1, Particle &dau2) const;

  /// @brief Gets the mass of the particle.
  /// @return
  double GetMass() const;

  /// @brief Adds a new particle type to the memory.
  /// @param name Name of the particle.
  /// @param mass
  /// @param charge
  /// @param width
  /// @return
  static int AddParticleType(const char *name, double mass, int charge,
                             double width = 0);

  /// @brief Resets particle types.
  static void ResetParticleTypes();

  /// @brief Prints all types of particles available.
  static void PrintAllTypes();

  /// @brief Gets particle type by its index.
  /// @param index
  /// @return
  static ParticleType *GetParticleType(const int index);

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