#include "predator.hpp"

#include "boid.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <cmath>
#include <random>
#include <vector>

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

const Boid* Predator::findPrey(const Swarm& swarm) const {
  const Boid* nearest_prey     = nullptr;
  double      nearest_distance = screen_.norm();

  for (int i = 0; i < swarm.size(); ++i) {
    const Boid&  current_boid = swarm[i];
    const double dist =
        distance(toroidal_, current_boid.position(), position(), screen_);
    if (dist < nearest_distance && dist <= attack_range_) {
      nearest_distance = dist;
      nearest_prey     = &current_boid;
    }
  }
  return nearest_prey;
}

void Predator::attack(Swarm& swarm) {
  const Boid* prey = findPrey(swarm);
  if (prey) {
    Vec3 prey_position = prey->position();
    Vec3 direction_to_prey =
        vecDistance(toroidal_, prey_position, position(), screen_).normalize();
    set_velocity(direction_to_prey * attack_speed_);
  }
}

Predator::Predator()
    : Boid()
    , attack_range_()
    , attack_speed_()
    , preferred_height_(100)
    , screen_(Vec3(600, 300, 300))
    , wind_(Vec3())
    , toroidal_() {}

Predator::Predator(const Predator& other)
    : Boid(other)
    , attack_range_(other.attack_range_)
    , attack_speed_(other.attack_speed_)
    , preferred_height_(other.preferred_height_)
    , screen_(other.screen_)
    , wind_(other.wind_)
    , toroidal_(other.toroidal_) {}

Predator::Predator(const GlobalVariables&   global_vars,
                   const PredatorVariables& predator_vars)
    : Boid(Vec3(0, 0, 0), Vec3(0, 0, 0))
    , attack_range_(predator_vars.attack_range)
    , attack_speed_(predator_vars.attack_speed)
    , preferred_height_(global_vars.screen.z() / 3)
    , screen_(global_vars.screen)
    , wind_(global_vars.wind)
    , toroidal_(global_vars.toroidal_bool) {
  if (attack_range_ <= 0) {
    throw std::invalid_argument("attack_range must be greater than 0");
  }
  if (attack_speed_ <= 0) {
    throw std::invalid_argument("attack_speed must be greater than 0");
  }

  Init();
}

void Predator::updatePredator(Swarm& swarm) {
  if (swarm.cooldown() >= 1000) {
    attack(swarm);
  } else {
    updateBoidVelocity(maintainHeight(*this, preferred_height_, 100),
                       attack_speed_ * 0.6);
  }
  updateBoidVelocity(Vec3(), attack_speed_);
  updateBoid(wind_, wind_.norm() + attack_speed_);

  border(screen_, toroidal_, *this);
}
} // namespace boids