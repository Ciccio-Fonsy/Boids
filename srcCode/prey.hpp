#ifndef PREY_HPP
#define PREY_HPP

#include "vec3.hpp"
#include "boid.hpp"

namespace boids {
class Prey : public Boid {
 public:
  Prey();
  Prey(Vec3 position, Vec3 velocity);

  void resetCooldown();
};

inline void Prey::resetCooldown() {};
} // namespace boids

#endif // PREY_HPP