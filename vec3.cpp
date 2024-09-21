#include "vec3.hpp"

#include <cmath>
#include <stdexcept>

namespace boids {
Vec3::Vec3()
    : x_(0)
    , y_(0)
    , z_(0) {}

Vec3::Vec3(const Vec3& other)
    : x_(other.x_)
    , y_(other.y_)
    , z_(other.z_) {}

Vec3::Vec3(double x, double y, double z)
    : x_(x)
    , y_(y)
    , z_(z) {}

double Vec3::norm() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }

Vec3 Vec3::normalize() const {
  double n = norm();
  if (n != 0) {
    return Vec3(x_ / n, y_ / n, z_ / n);
  } else {
    return *this;
  }
}

Vec3& Vec3::normalizeAssign() {
  *this = normalize();
  return *this;
}

double Vec3::dot(const Vec3& other) const {
  return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

Vec3 Vec3::cross(const Vec3& other) const {
  return Vec3(y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_,
              x_ * other.y_ - y_ * other.x_);
}
} // namespace boids