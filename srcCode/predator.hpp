#ifndef PREDATOR_HPP
#define PREDATOR_HPP

#include "boid.hpp"
#include "prey.hpp"
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
  int          cooldown_;

  void        init();
  const Prey* findPrey(const Swarm& swarm) const;
  void        attack(Swarm& swarm);
  Vec3        height() const;
  Vec3        circle(double r) const;
  void        stall(const Vec3& wind, double max_speed);

 public:
  Predator();
  Predator(const GlobalVariables&   global_vars,
           const PredatorVariables& predator_vars);

  double      attack_range() const;
  double      attack_speed() const;
  double      preferred_height() const;
  double      height_factor() const;
  const Vec3& screen() const;
  const Vec3& wind() const;
  bool        toroidal() const;
  int         cooldown() const;

  void resetCooldown();
  void updatePredator(Swarm& swarm);
};

inline double Predator::attack_range() const { return attack_range_; }

inline double Predator::attack_speed() const { return attack_speed_; }

inline double Predator::preferred_height() const { return preferred_height_; }

inline double Predator::height_factor() const { return height_factor_; }

inline const Vec3& Predator::screen() const { return screen_; }

inline const Vec3& Predator::wind() const { return wind_; }

inline bool Predator::toroidal() const { return toroidal_; }

inline int Predator::cooldown() const { return cooldown_; }

inline void Predator::resetCooldown() { cooldown_ = 0; }

inline void Predator::stall(const Vec3& wind, double max_speed) {
  if ((velocity() - wind).norm() <= 0.2 * max_speed) {
    updateBoidVelocity(
        wind,
        Vec3((velocity() - wind).x_, (velocity() - wind).y_, 0.2 * max_speed),
        max_speed);
  }
}
} // namespace boids

#endif // PREDATOR_HPP