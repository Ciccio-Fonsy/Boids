#ifndef BOID_HPP
#define BOID_HPP

#include "vec3.hpp"

//#include <memory> //serve per gli unique pointer (ma nion ci sono)

namespace boids {
class Boid {
  Vec3 position_;
  Vec3 velocity_;

 protected:
  Boid();
  Boid(Vec3 position, Vec3 velocity);

 public:
  virtual ~Boid() = default; //distruttore di default

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

  virtual void stall(const Vec3& wind, double max_speed) = 0; //funzioni vitruali pure
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
} // namespace boids

#endif // BOID_HPP
