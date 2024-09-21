#include "boid.hpp"

#include "vec3.hpp"

namespace boids {
Boid::Boid()
    : position_(Vec3(0, 0, 0))
    , velocity_(Vec3(0, 0, 0)) {}

Boid::Boid(const Boid& other)
    : position_(other.position_)
    , velocity_(other.velocity_) {}

Boid::Boid(Vec3 position, Vec3 velocity)
    : position_(position)
    , velocity_(velocity) {}

void Boid::updateBoidVelocity(Vec3 delta_v, double max_speed) {
  velocity_ += delta_v;

  if (velocity_.norm() > max_speed) {
    velocity_ = velocity_.normalize() * max_speed;
  }
}

void Boid::updateBoid(Vec3 delta_v, double max_speed) {
  updateBoidVelocity(delta_v, max_speed);
  position_ += velocity_;
}
} // namespace boids