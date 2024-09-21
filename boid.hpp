#ifndef BOID_H
#define BOID_H

#include "vec3.hpp"

namespace boids {
class Boid {
 private:
  Vec3 position_;
  Vec3 velocity_;

 public:
  Boid();
  Boid(const Boid& other);
  Boid(Vec3 position, Vec3 velocity);

  Vec3 position() const;
  Vec3 velocity() const;
  void set_position(const Vec3& new_position);
  void set_velocity(const Vec3& new_velocity);

  bool  operator==(const Boid& other) const;
  bool  operator!=(const Boid& other) const;
  Boid& operator=(const Boid& other);

  void updateBoidVelocity(Vec3 delta_v, double max_speed);
  void updateBoid(Vec3 delta_v, double max_speed);
};

inline Vec3 Boid::position() const { return position_; }

inline Vec3 Boid::velocity() const { return velocity_; }

inline void Boid::set_position(const Vec3& new_position) {
  position_ = new_position;
}

inline void Boid::set_velocity(const Vec3& new_velocity) {
  velocity_ = new_velocity;
}

inline bool Boid::operator==(const Boid& other) const {
  return position_ == other.position_ && velocity_ == other.velocity_;
}

inline bool Boid::operator!=(const Boid& other) const {
  return !(*this == other);
}

inline Boid& Boid::operator=(const Boid& other) {
  if (this != &other) {
    position_ = other.position_;
    velocity_ = other.velocity_;
  }
  return *this;
}
} // namespace boids

#endif // BOID_H