#ifndef STATISTICS_H
#define STATISTICS_H

#include "boid.hpp"
#include "vec3.hpp"

#include <cmath>
#include <numeric>
#include <vector>

namespace boids {
// From b to a
inline Vec3 vecDistance(bool toroidal, const Vec3& a, const Vec3& b,
                        const Vec3& width) {
  if (toroidal) {
    double dx = a.x() - b.x();
    double dy = a.y() - b.y();
    double dz = a.z() - b.z();

    if (std::abs(dx) > width.x() / 2) {
      dx += (dx > 0) ? -width.x() : width.x();
    }
    if (std::abs(dy) > width.y() / 2) {
      dy += (dy > 0) ? -width.y() : width.y();
    }

    Vec3 dist(dx, dy, dz);
    return dist;
  }
  return a - b;
}

inline double distance(bool toroidal, const Vec3& a, const Vec3& b,
                       const Vec3& width) {
  return vecDistance(toroidal, a, b, width).norm();
}

inline double mean(const std::vector<double>& values) {
  if (values.empty()) { throw std::runtime_error("No boids"); }
  return std::accumulate(values.begin(), values.end(), 0.0)
       / static_cast<double>(values.size());
}

inline double stdDev(const std::vector<double>& values, double mean_value) {
  double sum = std::accumulate(values.begin(), values.end(), 0.0,
                               [mean_value](double a, double b) {
                                 return a + (b - mean_value) * (b - mean_value);
                               });
  return std::sqrt(sum / static_cast<double>(values.size()));
}

inline void border(const Vec3& screen, bool toroidal, Boid& boid) {
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
      if (boid.position()[j] < 0) {
        new_position[j] = 0;
        if (boid.velocity()[j] < 0) { bounce[j] *= -0.9; }
      } else if (boid.position()[j] > screen[j]) {
        new_position[j] = screen[j];
        if (boid.velocity()[j] > 0) { bounce[j] *= -0.9; }
      }
    }
  }

  if (boid.position()[2] < 0) {
    new_position.set_z(0);
    if (boid.velocity()[2] < 0) { bounce[2] *= -0.9; }
  } else if (boid.position().z() > screen.z()) {
    new_position.set_z(screen.z());
    if (boid.velocity()[2] > 0) { bounce[2] *= -0.1; }
  }

  boid.set_velocity(bounce);
  boid.set_position(new_position);
}

inline Vec3 maintainHeight(Boid& boid, double target_height,
                           double division_factor) {
  Vec3 correction(0, 0, 0);
  if (std::abs(boid.position().z() - target_height) != 0
      && (boid.position().z() - target_height) * boid.velocity().z() > 0) {
    correction.set_z(target_height - boid.position().z());
  }
  return correction / division_factor;
}
} // namespace boids

#endif // STATISTICS_H