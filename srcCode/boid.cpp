#include "boid.hpp"

#include "vec3.hpp"

namespace boids {
Boid::Boid()
    : position_()
    , velocity_() {}

Boid::Boid(Vec3 position, Vec3 velocity)
    : position_(position)
    , velocity_(velocity) {}

void Boid::border(const Vec3& screen, bool toroidal) {
  Vec3 bounce       = velocity_;
  Vec3 new_position = position_;

  for (int j = 0; j < 2; ++j) {
    if (toroidal) {
      if (new_position[j] < 0) {
        new_position[j] = screen[j] + new_position[j];
      } else if (new_position[j] > screen[j]) {
        new_position[j] -= screen[j];
      }
    } else {
      if (new_position[j] < 0) {
        new_position[j] *= -0.5;
        if (bounce[j] < 0) { bounce[j] *= -0.5; }
      } else if (new_position[j] > screen[j]) {
        new_position[j] = 1.5 * screen[j] - 0.5 * new_position[j];
        if (bounce[j] > 0) { bounce[j] *= -0.5; }
      }
    }
  }

  if (new_position[2] < 0) {
    new_position[2] *= -0.5;
    if (bounce[2] < 0) { bounce[2] *= -0.5; }
  } else if (new_position[2] > screen[2]) {
    new_position[2] = 1.5 * screen[2] - 0.5 * new_position[2];
    if (bounce[2] > 0) { bounce[2] *= -0.1; }
  }

  velocity_ = bounce;
  position_ = new_position;
}
} // namespace boids