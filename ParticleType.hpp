#ifndef PARTICLE_TYPE_H
#define PARTICLE_TYPE_H

class ParticleType {

public:
  ParticleType(const char *name, const double mass, const int charge);

  const char *getFName() const;

  double getFMass() const;

  double getFCharge() const;

  virtual void print() const;

  virtual double GetWidth() const;

private:
  const char *fName_;
  const double fMass_;
  const int fCharge_;
};

#endif