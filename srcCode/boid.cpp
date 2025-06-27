#include "boid.hpp"

#include "vec3.hpp"

namespace boids {
Boid::Boid()
    : position_()
    , velocity_() {}

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
  set_position(position() + velocity());
}

void Boid::border(const Vec3& screen, bool toroidal) {
  Vec3 bounce       = velocity_;
  Vec3 new_position = position_;

  for (int j = 0; j < 2; ++j) {
    if (toroidal) {
      if (new_position[j] > screen[j]) {
        new_position[j] -= screen[j];
      } else if (new_position[j] < 0) {
        new_position[j] = screen[j] - new_position[j];
      }
    } else {
      if (new_position[j] < 0) {
        new_position[j] = 0;
        if (bounce[j] < 0) { bounce[j] *= -0.9; }
      } else if (new_position[j] > screen[j]) {
        new_position[j] = screen[j];
        if (bounce[j] > 0) { bounce[j] *= -0.9; }
      }
    }
  }

  if (new_position.z() < 0) {
    new_position.set_z(0);
    if (bounce.z() < 0) { bounce.set_z(bounce.z() * -0.9); }
  } else if (new_position.z() > screen.z()) {
    new_position.set_z(screen.z());
    if (bounce.z() > 0) { bounce.set_z(bounce.z() * -0.1); }
  }

  velocity_ = bounce;
  position_ = new_position;
}
} // namespace boids