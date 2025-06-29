#include "predator.hpp"

#include "boid.hpp"
#include "prey.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <random>
#include <stdexcept>

namespace boids {
void Predator::init() {
  std::random_device               rd;
  std::mt19937                     gen(rd());
  std::uniform_real_distribution<> ds(0, 1.0);
  std::uniform_real_distribution<> dv(-1.0, 1.0);

  const double x = ds(gen) * screen_.x_;
  const double y = ds(gen) * screen_.y_;

  set_position(Vec3(x, y, 0));

  const double max_deviation = attack_speed_ / std::sqrt(3);

  const double vx = dv(gen) * max_deviation;
  const double vy = dv(gen) * max_deviation;
  const double vz = dv(gen) * max_deviation;

  set_velocity(Vec3(vx, vy, vz));

  if (velocity().norm() > attack_speed_) {
    set_velocity(velocity().normalize() * attack_speed_);
  }
}

const Prey* Predator::findPrey(const Swarm& swarm) const {
  const Prey* nearest_prey     = nullptr;
  double      nearest_distance = screen_.norm();

  for (int i = 0; i < swarm.size(); ++i) {
    const Prey&  current_prey = swarm[i];
    const double dist =
        current_prey.position().distance(toroidal_, position(), screen_);
    if (dist < nearest_distance && dist <= attack_range_) {
      nearest_distance = dist;
      nearest_prey     = &current_prey;
    }
  }
  return nearest_prey;
}

void Predator::attack(Swarm& swarm) {
  const Prey* prey = findPrey(swarm);
  if (prey) {
    Vec3 prey_position = prey->position();
    Vec3 direction_to_prey =
        prey_position.vecDistance(toroidal_, position(), screen_).normalize();
    set_velocity(direction_to_prey * attack_speed_);
  }
}

Vec3 Predator::height() const {
  Vec3   correction;
  double dh     = preferred_height_ - position().z_;
  double v_norm = velocity().norm();

  if (v_norm == 0 || dh * velocity().z_ / std::abs(dh) / v_norm <= 0.5) {
    correction.z_ = dh;
  }

  return correction * height_factor_;
}

Vec3 Predator::circle(double r) const {
  Vec3 v = velocity();

  return Vec3(v.y_, -v.x_, 0) * v.norm() / r;
}

Predator::Predator()
    : Boid()
    , attack_range_()
    , attack_speed_()
    , preferred_height_(100)
    , height_factor_(0.01)
    , screen_(Vec3(600, 300, 300))
    , wind_()
    , toroidal_()
    , cooldown_() {}

Predator::Predator(const GlobalVariables&   global_vars,
                   const PredatorVariables& predator_vars)
    : Boid()
    , attack_range_(predator_vars.attack_range)
    , attack_speed_(predator_vars.attack_speed)
    , preferred_height_(global_vars.screen.z_ / 3)
    , height_factor_(0.01)
    , screen_(global_vars.screen)
    , wind_(global_vars.wind)
    , toroidal_(global_vars.toroidal_bool)
    , cooldown_() {
  if (attack_range_ <= 0) {
    throw std::invalid_argument("attack_range must be greater than 0");
  }
  if (attack_speed_ <= 0) {
    throw std::invalid_argument("attack_speed must be greater than 0");
  }

  init();
}

void Predator::updatePredator(Swarm& swarm) {
  if (cooldown_ >= 1000) {
    attack(swarm);
  } else {
    ++cooldown_;

    updateBoidVelocity(wind_, height() + circle(screen_.norm() / 4),
                       attack_speed_ / 2);
  }
  stall(wind_, attack_speed_);
  updateBoid(wind_, wind_, attack_speed_);

  border(screen_, toroidal_);
}
} // namespace boids