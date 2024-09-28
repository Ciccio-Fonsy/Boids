#ifndef PREDATOR_HPP
#define PREDATOR_HPP

#include "boid.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

namespace boids {
class Predator : public Boid {
  const double attack_range_;
  const double attack_speed_;
  const double preferred_height_;
  const double height_factor_;
  const Vec3   screen_;
  const Vec3   wind_;
  const bool   toroidal_;

  void        Init();
  const Boid* findPrey(const Swarm& swarm) const;
  void        attack(Swarm& swarm);

 public:
  Predator();
  Predator(const Predator& other);
  Predator(const GlobalVariables&   global_vars,
           const PredatorVariables& predator_vars);

  Predator& operator=(const Predator& other);

  double      attack_range() const;
  double      attack_speed() const;
  double      preferred_height() const;
  double      height_factor() const;
  const Vec3& screen() const;
  const Vec3& wind() const;
  bool        toroidal() const;

  void updatePredator(Swarm& swarm);
};

inline Predator& Predator::operator=(const Predator& other) {
  if (this != &other) {
    set_position(other.position());
    set_velocity(other.velocity());
  }
  return *this;
}

inline double Predator::attack_range() const { return attack_range_; }

inline double Predator::attack_speed() const { return attack_speed_; }

inline double Predator::preferred_height() const { return preferred_height_; }

inline double Predator::height_factor() const { return height_factor_; }

inline const Vec3& Predator::screen() const { return screen_; }

inline const Vec3& Predator::wind() const { return wind_; }

inline bool Predator::toroidal() const { return toroidal_; }
} // namespace boids

#endif // PREDATOR_HPP