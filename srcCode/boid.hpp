#ifndef BOID_HPP
#define BOID_HPP

#include "vec3.hpp"

#include <memory>

namespace boids {
class Boid {
  Vec3 position_;
  Vec3 velocity_;

 protected:
  Boid();
  Boid(Vec3 position, Vec3 velocity);

 public:
  virtual ~Boid() = default;

  Vec3 position() const;
  Vec3 velocity() const;
  void set_position(const Vec3& new_position);
  void set_velocity(const Vec3& new_velocity);

  bool operator==(const Boid& other) const;
  bool operator!=(const Boid& other) const;

  void updateBoidVelocity(const Vec3& wind, const Vec3& delta_v,
                          double max_speed);
  void updateBoid(const Vec3& wind, const Vec3& delta_v, double max_speed);

  void border(const Vec3& screen, bool toroidal);
  Vec3 maintainHeight(double target_height, double height_factor) const;

  virtual void stall(const Vec3& wind, double max_speed) = 0;
  virtual void resetCooldown()                           = 0;
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
  return position() == other.position() && velocity() == other.velocity();
}

inline bool Boid::operator!=(const Boid& other) const {
  return !(*this == other);
}

inline void Boid::updateBoidVelocity(const Vec3& wind, const Vec3& delta_v,
                                     double max_speed) {
  velocity_ += delta_v;

  if ((velocity_ - wind).norm() > max_speed) {
    velocity_ = (velocity_ - wind).normalize() * max_speed + wind;
  }
}

inline void Boid::updateBoid(const Vec3& wind, const Vec3& delta_v,
                             double max_speed) {
  updateBoidVelocity(wind, delta_v, max_speed);
  set_position(position() + velocity());
}

inline Vec3 Boid::maintainHeight(double target_height,
                                 double height_factor) const {
  Vec3 correction;
  if (std::abs(position_.z() - target_height) != 0
      && (position_.z() - target_height) * velocity_.z() > 0) {
    correction.set_z(target_height - position_.z());
  }
  return correction * height_factor;
}
} // namespace boids

#endif // BOID_HPP