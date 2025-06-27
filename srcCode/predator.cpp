#include "predator.hpp"

#include "boid.hpp"
#include "prey.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <iostream>
#include <random>
#include <stdexcept>

namespace boids {
void Predator::Init() {
  std::random_device               rd;
  std::mt19937                     gen(rd());
  std::uniform_real_distribution<> ds(0, 1.0);
  std::uniform_real_distribution<> dv(-1.0, 1.0);

  const double x = ds(gen) * screen_.x();
  const double y = ds(gen) * screen_.y();
  const double z = ds(gen) * screen_.z();

  set_position(Vec3(x, y, z));

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
    , preferred_height_(global_vars.screen.z() / 3)
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

  Init();
}

void Predator::updatePredator(Swarm& swarm) {
  if (cooldown_ >= 1000) {
    attack(swarm);
  } else {
    ++cooldown_;

    updateBoidVelocity(maintainHeight(preferred_height_, height_factor_),
                       attack_speed_ / 2);
  }
  updateBoidVelocity(Vec3(), attack_speed_);
  updateBoid(wind_, wind_.norm() + attack_speed_);

  border(screen_, toroidal_);
}
} // namespace boids