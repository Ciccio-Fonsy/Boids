#include "vec3.hpp"

#include <cmath>

namespace boids {
Vec3::Vec3()
    : x_()
    , y_()
    , z_() {}

Vec3::Vec3(double x, double y, double z)
    : x_(x)
    , y_(y)
    , z_(z) {}

Vec3 Vec3::vecDistance(bool toroidal, const Vec3& other,
                       const Vec3& width) const { // From b to a
  if (toroidal) {
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    double dz = z_ - other.z_;

    if (std::abs(dx) > width.x_ / 2) { dx += (dx > 0) ? -width.x_ : width.x_; }
    if (std::abs(dy) > width.y_ / 2) { dy += (dy > 0) ? -width.y_ : width.y_; }

    Vec3 dist(dx, dy, dz);
    return dist;
  }
  return *this - other;
}
} // namespace boids