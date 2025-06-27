#include "statistics.hpp"

#include "boid.hpp"
#include "vec3.hpp"

namespace boids {
void border(const Vec3& screen, bool toroidal, Boid& boid) {
  Vec3 bounce       = boid.velocity();
  Vec3 new_position = boid.position();

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

  boid.set_velocity(bounce);
  boid.set_position(new_position);
}
} // namespace boids