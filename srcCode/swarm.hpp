#ifndef SWARM_HPP
#define SWARM_HPP

#include "boid.hpp"
#include "prey.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <cmath>
#include <vector>

namespace boids {
class Swarm {
  std::vector<Prey> preys_;
  int               size_;
  const double      wingspan_;
  const double      max_speed_;
  const double      min_distance_;
  const double      sight_distance_;
  const double      visual_field_;
  const double      preferred_height_;
  const double      separation_factor_;
  const double      cohesion_factor_;
  const double      alignment_factor_;
  const double      fear_factor_;
  const double      height_factor_;
  const Vec3        screen_;
  const Vec3        wind_;
  const bool        toroidal_;
  Boid*             predator_;

  bool isWithinRange(const Boid& b1, const Boid& b2, double range) const;
  bool isWithinField(const Boid& b1, const Boid& b2) const;

  void Init();
  void bounce(Prey& b);
  Vec3 separation(const Prey& b) const;
  Vec3 cohesion(const Prey& b) const;
  Vec3 alignment(const Prey& b) const;
  Vec3 fear(const Prey& b) const;

 public:
  Swarm();
  Swarm(const GlobalVariables& global_vars, const SwarmVariables& swarm_vars,
        Boid* predator);

  Prey&       operator[](int i);
  const Prey& operator[](int i) const;

  int         size() const;
  double      wingspan() const;
  double      max_speed() const;
  double      min_distance() const;
  double      sight_distance() const;
  double      visual_field() const;
  double      preferred_height() const;
  double      separation_factor() const;
  double      cohesion_factor() const;
  double      alignment_factor() const;
  double      fear_factor() const;
  double      height_factor() const;
  const Vec3& screen() const;
  const Vec3& wind() const;
  bool        toroidal() const;

  void updateSwarm();
};

inline bool Swarm::isWithinRange(const Boid& b1, const Boid& b2,
                                 double range) const {
  return b1.position().distance(toroidal_, b2.position(), screen_) <= range;
}

inline bool Swarm::isWithinField(const Boid& observer,
                                 const Boid& target) const {
  return observer.velocity().norm() == 0
      || observer.velocity().normalize().dot(
             target.position()
                 .vecDistance(toroidal_, observer.position(), screen_)
                 .normalize())
             >= std::cos(visual_field_);
}

inline Prey& Swarm::operator[](int i) {
  if (i < 0 || i >= size_) { throw std::out_of_range("Index out of range"); }
  return preys_[static_cast<std::size_t>(i)];
}

inline const Prey& Swarm::operator[](int i) const {
  if (i < 0 || i >= size_) { throw std::out_of_range("Index out of range"); }
  return preys_[static_cast<std::size_t>(i)];
}

inline int Swarm::size() const { return static_cast<int>(size_); }

inline double Swarm::wingspan() const { return wingspan_; }

inline double Swarm::max_speed() const { return max_speed_; }

inline double Swarm::min_distance() const { return min_distance_; }

inline double Swarm::sight_distance() const { return sight_distance_; }

inline double Swarm::visual_field() const { return visual_field_; }

inline double Swarm::preferred_height() const { return preferred_height_; }

inline double Swarm::separation_factor() const { return separation_factor_; }

inline double Swarm::cohesion_factor() const { return cohesion_factor_; }

inline double Swarm::alignment_factor() const { return alignment_factor_; }

inline double Swarm::fear_factor() const { return fear_factor_; }

inline double Swarm::height_factor() const { return height_factor_; }

inline const Vec3& Swarm::screen() const { return screen_; }

inline const Vec3& Swarm::wind() const { return wind_; }

inline bool Swarm::toroidal() const { return toroidal_; }
} // namespace boids

#endif // SWARM_HPP