#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace boids {
struct Vec3 {
  double x_;
  double y_;
  double z_;

  Vec3();
  Vec3(double x, double y, double z);

  bool   operator==(const Vec3& other) const;
  bool   operator!=(const Vec3& other) const;
  Vec3   operator+(const Vec3& other) const;
  Vec3&  operator+=(const Vec3& other);
  Vec3   operator-(const Vec3& other) const;
  Vec3&  operator-=(const Vec3& other);
  Vec3   operator*(double scalar) const; //vec*n
  Vec3&  operator*=(double scalar);
  Vec3   operator/(double scalar) const;
  Vec3&  operator/=(double scalar);
  double norm() const;
  Vec3   normalize() const;
  Vec3&  normalizeAssign();
  double dot(const Vec3& other) const; //prodotto scalare
  Vec3   cross(const Vec3& other) const; //prodotto vettoriale
  Vec3   vecDistance(bool toroidal, const Vec3& other, const Vec3& width) const; //distanza vettoriale con possibilitàò toriodale
  double distance(bool toroidal, const Vec3& other, const Vec3& width) const; //distanza scalare con possibilità toroidale
  double operator[](int i) const;
  double& operator[](int i);

  friend Vec3          operator*(double scalar, const Vec3& v); //per completezza se no n*vec3 non è definito
  friend std::ostream& operator<<(std::ostream& os, const Vec3& v); //sostituisce converte in una stringa quyando chiedi di stampare un vettore
};

inline Vec3::Vec3() //costruttore di default
    : x_()
    , y_()
    , z_() {}

inline Vec3::Vec3(double x, double y, double z)
    : x_(x)
    , y_(y)
    , z_(z) {}

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
  x_ += other.x_;
  y_ += other.y_;
  z_ += other.z_;
  return *this;
}

inline Vec3 Vec3::operator-(const Vec3& other) const {
  return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

inline Vec3& Vec3::operator-=(const Vec3& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  z_ -= other.z_;
  return *this;
}

inline Vec3 Vec3::operator*(double scalar) const {
  return Vec3(x_ * scalar, y_ * scalar, z_ * scalar);
}

inline Vec3& Vec3::operator*=(double scalar) {
  x_ *= scalar;
  y_ *= scalar;
  z_ *= scalar;
  return *this;
}

inline Vec3 Vec3::operator/(double scalar) const {
  if (scalar == 0) { throw std::domain_error("Division by zero."); }
  return Vec3(x_ / scalar, y_ / scalar, z_ / scalar);
}

inline Vec3& Vec3::operator/=(double scalar) {
  if (scalar == 0) { throw std::domain_error("Division by zero."); }
  x_ /= scalar;
  y_ /= scalar;
  z_ /= scalar;
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

inline Vec3 operator*(double scalar, const Vec3& v) { return v * scalar; }

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {   //una qualsiasi forma di output: passare ostream significa che 
                                                                    //a sinistra ho una stringa da srtampare a cui aggiungo vec3
                                                                    //mi restituisce quella stringa a vui ha aggiunto vec3
  os << "(" << v.x_ << ", " << v.y_ << ", " << v.z_ << ")";
  return os;
}
} // namespace boids

#endif // VEC3_HPP
