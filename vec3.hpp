#ifndef VEC3_H
#define VEC3_H

#include <stdexcept>

namespace boids {
class Vec3 {
 private:
  double x_;
  double y_;
  double z_;

 public:
  Vec3();
  Vec3(const Vec3& other);
  Vec3(double x, double y, double z);

  double x() const;
  double y() const;
  double z() const;
  void   set_x(double x);
  void   set_y(double y);
  void   set_z(double z);

  bool    operator==(const Vec3& other) const;
  bool    operator!=(const Vec3& other) const;
  Vec3&   operator=(const Vec3& other);
  Vec3    operator+(const Vec3& other) const;
  Vec3&   operator+=(const Vec3& other);
  Vec3    operator-(const Vec3& other) const;
  Vec3&   operator-=(const Vec3& other);
  Vec3    operator*(double scalar) const;
  Vec3&   operator*=(double scalar);
  Vec3    operator/(double scalar) const;
  Vec3&   operator/=(double scalar);
  double  norm() const;
  Vec3    normalize() const;
  Vec3&   normalizeAssign();
  double  dot(const Vec3& other) const;
  Vec3    cross(const Vec3& other) const;
  double  operator[](int i) const;
  double& operator[](int i);

  std::string toString() const;
};

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

inline Vec3& Vec3::operator=(const Vec3& other) {
  if (this != &other) {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
  }
  return *this;
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

inline double Vec3::operator[](int i) const {
  if (i < 0 || i > 2) { throw std::out_of_range("Index out of range"); }
  return (i == 0) ? x_ : (i == 1) ? y_ : z_;
}

inline double& Vec3::operator[](int i) {
  if (i < 0 || i > 2) { throw std::out_of_range("Index out of range"); }
  return (i == 0) ? x_ : (i == 1) ? y_ : z_;
}

inline std::string Vec3::toString() const {
  return "(" + std::to_string(x_) + ", " + std::to_string(y_) + ", "
       + std::to_string(z_) + ")";
}
} // namespace boids

#endif // VEC3_H