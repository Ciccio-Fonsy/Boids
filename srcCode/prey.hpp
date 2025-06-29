#ifndef PREY_HPP
#define PREY_HPP

#include "boid.hpp"
#include "vec3.hpp"

namespace boids {
class Prey : public Boid {
 public:
  Prey();
  Prey(Vec3 position, Vec3 velocity);

  void resetCooldown();
  void stall(const Vec3& wind, double max_speed);
};

inline Prey::Prey()
    : Boid() {}

inline Prey::Prey(Vec3 position, Vec3 velocity)
    : Boid(position, velocity) {}

inline void Prey::stall(const Vec3& wind, double max_speed) {
  if ((velocity() - wind).norm() <= 0.4 * max_speed) {
    updateBoidVelocity(
        wind,
        Vec3((velocity() - wind).x_, (velocity() - wind).y_, 0.4 * max_speed),
        max_speed);
  }
}

inline void Prey::resetCooldown() {}
} // namespace boids

#endif // PREY_HPP