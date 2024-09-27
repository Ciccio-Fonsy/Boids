#include "swarm.hpp"

#include "boid.hpp"
#include "statistics.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <random>
#include <stdexcept>
#include <vector>

namespace boids {
void Swarm::Init() {
  std::random_device               rd;
  std::mt19937                     gen(rd());
  std::uniform_real_distribution<> ds(0, 1.0);
  std::uniform_real_distribution<> dv(-1.0, 1.0);

  for (Boid& b : boids_) {
    const double x = ds(gen) * screen_.x();
    const double z = ds(gen) * screen_.z();
    const double y = ds(gen) * screen_.y();

    b.set_position(Vec3(x, y, z));

    double max_deviation = max_speed_ / std::sqrt(3);

    const double vx = dv(gen) * max_deviation;
    const double vy = dv(gen) * max_deviation;
    const double vz = dv(gen) * max_deviation;

    b.set_velocity(Vec3(vx, vy, vz));

    if (b.velocity().norm() > max_speed_) {
      b.set_velocity(b.velocity().normalize() * max_speed_);
    }
  }
}

void Swarm::bounce(Boid& b) {
  for (Boid& other_boid : boids_) {
    if (other_boid != b && isWithinRange(other_boid, b, 2 * wingspan_)) {
      if (b.position() == other_boid.position()) {
        b.set_position(b.position() + Vec3(0, 0, wingspan_ / 10));
      }

      const Vec3 b_position     = b.position();
      const Vec3 other_position = other_boid.position();
      const Vec3 separation_vector =
          vecDistance(toroidal_, other_position, b_position, screen_);
      const Vec3 b_velocity        = b.velocity();
      const Vec3 other_velocity    = other_boid.velocity();
      const Vec3 relative_velocity = b_velocity - other_velocity;
      const bool approaching = separation_vector.dot(relative_velocity) > 0;

      if (approaching) {
        const Vec3 collision_normal = separation_vector.normalize();
        const Vec3 velocity_b_after =
            b_velocity
            - collision_normal * (relative_velocity.dot(collision_normal));
        const Vec3 velocity_other_after =
            other_velocity
            + collision_normal * (relative_velocity.dot(collision_normal));

        b.set_velocity(velocity_b_after * 0.1);
        other_boid.set_velocity(velocity_other_after * 0.1);
      }
    }
  }
}

Vec3 Swarm::separation(const Boid& b) const {
  Vec3 c;

  for (const Boid& other_boid : boids_) {
    if (other_boid != b && isWithinRange(other_boid, b, min_distance_)
        && other_boid.position() != b.position()) {
      const Vec3 separation_vector =
          vecDistance(toroidal_, other_boid.position(), b.position(), screen_);
      c -= separation_vector.normalize() / separation_vector.norm();
    }
  }

  return c * separation_factor_;
}

Vec3 Swarm::cohesion(const Boid& b) const {
  Vec3 perceived_center;
  int  count = 0;

  for (const Boid& other_boid : boids_) {
    if (other_boid != b && isWithinRange(b, other_boid, sight_distance_)) {
      Vec3 position = other_boid.position();

      if (toroidal_) {
        for (int j = 0; j < 2; ++j) {
          const double pos_diff = position[j] - perceived_center[j];

          if (std::abs(pos_diff) >= screen_[j] / 2) {
            position[j] += (pos_diff < 0 ? screen_[j] : -screen_[j]);
          }

          perceived_center[j] =
              (count * perceived_center[j] + position[j]) / (count + 1);
          perceived_center[j] =
              std::fmod(perceived_center[j] + screen_[j], screen_[j]);
        }

        perceived_center[2] += position[2];
      } else {
        perceived_center += position;
      }

      ++count;
    }
  }

  if (count == 0) { return Vec3(0, 0, 0); }

  if (toroidal_) {
    perceived_center[2] /= count;
  } else {
    perceived_center /= count;
  }

  return vecDistance(toroidal_, perceived_center, b.position(), screen_)
       * cohesion_factor_;
}

Vec3 Swarm::alignment(const Boid& b) const {
  Vec3 pv;
  int  count = 0;

  for (const Boid& other_boid : boids_) {
    if (other_boid != b && isWithinRange(other_boid, b, min_distance_)) {
      pv += other_boid.velocity();
      ++count;
    }
  }

  if (count == 0) { return Vec3(0, 0, 0); }

  pv /= count;
  return (pv - b.velocity()) * alignment_factor_;
}

Vec3 Swarm::fear(const Boid& b) const {
  Vec3       evade_vector;
  const Vec3 distance_to_predator =
      vecDistance(toroidal_, b.position(), predator_.position(), screen_);
  const double distance_norm = distance_to_predator.norm();

  if (distance_norm == 0) {
    evade_vector = b.velocity().normalize() * max_speed_;
  } else if (distance_norm <= sight_distance_) {
    const Vec3   flee_direction = distance_to_predator.normalize();
    const double evade_strength =
        (sight_distance_ - distance_norm) / sight_distance_;

    evade_vector = flee_direction * max_speed_ * evade_strength;
  }
  return evade_vector * fear_factor_;
}

Swarm::Swarm()
    : size_(100)
    , wingspan_(2)
    , max_speed_(1)
    , min_distance_(30)
    , sight_distance_(150)
    , preferred_height_(200)
    , separation_factor_(0.05)
    , cohesion_factor_(0.00005)
    , alignment_factor_(0.005)
    , fear_factor_(0.05)
    , predator_()
    , screen_(Vec3(600, 300, 300))
    , wind_()
    , toroidal_()
    , cooldown_() {
  for (int i = 0; i < size_; ++i) { boids_.push_back(Boid()); }

  Init();
}

Swarm::Swarm(const Swarm& other)
    : size_(other.size_)
    , wingspan_(other.wingspan_)
    , max_speed_(other.max_speed_)
    , min_distance_(other.min_distance_)
    , sight_distance_(other.sight_distance_)
    , preferred_height_(other.preferred_height_)
    , separation_factor_(other.separation_factor_)
    , cohesion_factor_(other.cohesion_factor_)
    , alignment_factor_(other.alignment_factor_)
    , fear_factor_(other.fear_factor_)
    , predator_(other.predator_)
    , screen_(other.screen_)
    , wind_(other.wind_)
    , toroidal_(other.toroidal_)
    , cooldown_(other.cooldown_) {
  for (int i = 0; i < size_; ++i) { boids_.push_back(Boid()); }

  Init();
}

Swarm::Swarm(const GlobalVariables& global_vars,
             const SwarmVariables& swarm_vars, const Boid& predator)
    : size_(swarm_vars.size)
    , wingspan_(swarm_vars.wingspan)
    , max_speed_(swarm_vars.max_speed)
    , min_distance_(swarm_vars.min_distance)
    , sight_distance_(swarm_vars.sight_distance)
    , preferred_height_(global_vars.screen.z() * 2 / 3)
    , separation_factor_(swarm_vars.separation_factor)
    , cohesion_factor_(swarm_vars.cohesion_factor)
    , alignment_factor_(swarm_vars.alignment_factor)
    , fear_factor_(swarm_vars.fear_factor)
    , predator_(predator)
    , screen_(global_vars.screen)
    , wind_(global_vars.wind)
    , toroidal_(global_vars.toroidal_bool)
    , cooldown_() {
  if (size_ <= 1) {
    throw std::invalid_argument("Swarm size must be greater than 1");
  }
  if (wingspan_ <= 0) {
    throw std::invalid_argument("Wingspan must be greater than 0");
  }
  if (max_speed_ <= 0) {
    throw std::invalid_argument("Maximum speed must be greater than 0");
  }
  if (min_distance_ <= 0) {
    throw std::invalid_argument("Minimum distance must be greater than 0");
  }
  if (sight_distance_ <= 0) {
    throw std::invalid_argument("Sight distance must be greater than 0");
  }
  if (separation_factor_ <= 0 || separation_factor_ > 100) {
    throw std::invalid_argument(
        "Separation factor must be greater than 0 and smaller than 100");
  }
  if (cohesion_factor_ <= 0 || cohesion_factor_ > 100) {
    throw std::invalid_argument(
        "Cohesion factor must be greater than 0 and smaller than 100");
  }
  if (alignment_factor_ <= 0 || alignment_factor_ > 100) {
    throw std::invalid_argument(
        "Alignment factor must be greater than 0 and smaller than 100");
  }
  if (fear_factor_ <= 0 || fear_factor_ > 100) {
    throw std::invalid_argument(
        "Fear factor must be greater than 0 and smaller than 100");
  }

  for (int i = 0; i < size_; ++i) { boids_.push_back(Boid()); }

  Init();
}

void Swarm::updateSwarm(Boid& predator) {
  ++cooldown_;
  predator_ = predator;

  std::vector<int> boids_to_remove;

  for (int i = 0; i < size_; ++i) {
    Boid& current_boid = boids_[static_cast<std::size_t>(i)];

    if (isWithinRange(predator_, current_boid, wingspan_)) {
      boids_to_remove.push_back(i);
      cooldown_ = 0;
    } else {
      const Vec3 v1 = separation(current_boid);
      const Vec3 v2 = cohesion(current_boid);
      const Vec3 v3 = alignment(current_boid);
      const Vec3 v4 = maintainHeight(current_boid, preferred_height_, 1000);

      bounce(current_boid);
      current_boid.updateBoidVelocity(v1 + v2 + v3 + v4, max_speed_);
      current_boid.updateBoidVelocity(fear(current_boid), max_speed_);
      current_boid.updateBoid(wind_, wind_.norm() + max_speed_);

      border(screen_, toroidal_, current_boid);
    }
  }

  for (auto it = boids_to_remove.rbegin(); it != boids_to_remove.rend(); ++it) {
    boids_.erase(boids_.begin() + static_cast<int>(*it));
    --size_;
  }
}
} // namespace boids