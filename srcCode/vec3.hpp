#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <stdexcept>

namespace boids {
class Vec3 {
  double x_;
  double y_;
  double z_;

 public:
  Vec3();
  Vec3(double x, double y, double z);

  double x() const;
  double y() const;
  double z() const;
  void   set_x(double x);
  void   set_y(double y);
  void   set_z(double z);

  bool   operator==(const Vec3& other) const;
  bool   operator!=(const Vec3& other) const;
  Vec3   operator+(const Vec3& other) const;
  Vec3&  operator+=(const Vec3& other);
  Vec3   operator-(const Vec3& other) const;
  Vec3&  operator-=(const Vec3& other);
  Vec3   operator*(double scalar) const;
  Vec3&  operator*=(double scalar);
  Vec3   operator/(double scalar) const;
  Vec3&  operator/=(double scalar);
  double norm() const;
  Vec3   normalize() const;
  Vec3&  normalizeAssign();
  double dot(const Vec3& other) const;
  Vec3   cross(const Vec3& other) const;
  Vec3   vecDistance(bool toroidal, const Vec3& other, const Vec3& width) const;
  double distance(bool toroidal, const Vec3& other, const Vec3& width) const;
  double operator[](int i) const;
  double& operator[](int i);

  std::string toString() const;
};

inline Vec3::Vec3()
    : x_()
    , y_()
    , z_() {}

inline Vec3::Vec3(double x, double y, double z)
    : x_(x)
    , y_(y)
    , z_(z) {}

inline double Vec3::x() const { return x_; }

inline double Vec3::y() const { return y_; }

inline double Vec3::z() const { return z_; }

inline void Vec3::set_x(double x) { x_ = x; }

inline void Vec3::set_y(double y) { y_ = y; }

inline void Vec3::set_z(double z) { z_ = z; }

inline bool Vec3::operator==(const Vec3& other) const {
  return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
}

inline bool Vec3::operator!=(const Vec3& other) const {
  return !(*this == other);
}

inline Vec3 Vec3::operator+(const Vec3& other) const {
  return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

inline Vec3& Vec3::operator+=(const Vec3& other) {
  *this = *this + other;
  return *this;
}

inline Vec3 Vec3::operator-(const Vec3& other) const {
  return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

inline Vec3& Vec3::operator-=(const Vec3& other) {
  *this = *this - other;
  return *this;
}

inline Vec3 Vec3::operator*(double scalar) const {
  return Vec3(x_ * scalar, y_ * scalar, z_ * scalar);
}

inline Vec3& Vec3::operator*=(double scalar) {
  *this = *this * scalar;
  return *this;
}

inline Vec3 Vec3::operator/(double scalar) const {
  if (scalar == 0) { throw std::invalid_argument("Division by zero."); }
  return Vec3(x_ / scalar, y_ / scalar, z_ / scalar);
}

inline Vec3& Vec3::operator/=(double scalar) {
  *this = *this / scalar;
  return *this;
}

inline double Vec3::norm() const {
  return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

inline Vec3 Vec3::normalize() const {
  double n = norm();
  if (n != 0) { return Vec3(x_ / n, y_ / n, z_ / n); }
  return *this;
}

inline Vec3& Vec3::normalizeAssign() {
  *this = normalize();
  return *this;
}

inline double Vec3::dot(const Vec3& other) const {
  return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

inline Vec3 Vec3::cross(const Vec3& other) const {
  return Vec3(y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_,
              x_ * other.y_ - y_ * other.x_);
}

inline double Vec3::operator[](int i) const {
  switch (i) {
  case 0 : return x_;
  case 1 : return y_;
  case 2 : return z_;
  default: throw std::out_of_range("Index out of range");
  }
}

inline double& Vec3::operator[](int i) {
  switch (i) {
  case 0 : return x_;
  case 1 : return y_;
  case 2 : return z_;
  default: throw std::out_of_range("Index out of range");
  }
}
} // namespace boids

#endif // VEC3_HPP