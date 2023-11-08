#ifndef RESONANCE_TYPE_H
#define RESONANCE_TYPE_H

#include "ParticleType.hpp"

class ResonanceType : public ParticleType {

public:
  ResonanceType(const char *name, double mass, int charge, double width);
  double GetWidth() const override;
  void print() const override;

private:
  double fWidth_;
};

#endif