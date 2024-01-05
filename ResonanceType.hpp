#ifndef RESONANCE_TYPE_H
#define RESONANCE_TYPE_H

#include "ParticleType.hpp"

class ResonanceType : public ParticleType {

public:
  ResonanceType(const char *name, const double mass, const int charge,
                const double width);
  double GetWidth() const override;
  void Print() const override;

private:
  const double fWidth_;
};

#endif