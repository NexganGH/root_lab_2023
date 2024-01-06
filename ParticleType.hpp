#ifndef PARTICLE_TYPE_H
#define PARTICLE_TYPE_H

class ParticleType {

public:
  ParticleType(const char *name, const double mass, const int charge);

  /// @brief Returns the name of the particle type.
  /// @return
  const char *GetName() const;

  /// @brief Returns the mass of the particle type.
  /// @return
  double GetMass() const;

  /// @brief Returns the charge of the particle type.
  /// @return
  double GetCharge() const;

  /// @brief Prints the particle type information.
  virtual void Print() const;

  /// @brief Returns the width of the particle type (set only if this is a
  /// resonance).
  /// @return
  virtual double GetWidth() const;

private:
  const char *fName_;
  const double fMass_;
  const int fCharge_;
};

#endif