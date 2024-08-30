#ifndef PREDATOR
#define PREDATOR

#include "boid.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "vec3.hpp"

#include <cmath>
#include <random>
#include <vector>

class Predator : public Boid
{
  double attack_range;
  double attack_speed;
  Vec3 screen;
  bool toroidal;
  double prefered_height;
  Vec3 wind;

  void initialize_predator()
  {
    // Initialize random number generator
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> dis(-1.0, 1.0); // Define the range

    std::uniform_real_distribution<> ds(0, 1.0); // Define the range

    double x = ds(gen) * screen.x;
    double y = ds(gen) * screen.y;
    double z = ds(gen) * screen.z;

    // Imposta la posizione del boid
    set_position(Vec3(x, y, z));

    // Calculate the maximum deviation
    double max_deviation = attack_speed / std::sqrt(3);

    // Generate random velocities within the desired range
    double vx = dis(gen) * 2 * max_deviation;
    double vy = dis(gen) * 2 * max_deviation;
    double vz = dis(gen) * 2 * max_deviation;

    set_velocity(Vec3(vx, vy, vz));

    if (get_velocity().norm() > attack_speed) {
      set_velocity(get_velocity().normalize() * attack_speed);
    };
  }

  const Boid* find_prey(const Swarm& boids)
  {
    const Boid* nearest_prey = nullptr;
    double nearest_distance  = screen.norm();

    for (std::vector<Boid>::size_type i = 0; i < boids.get_size(); ++i) {
      const Boid& current_boid = boids[i];
      double dist              = toroidal
                                   ? toroidal_distance(current_boid.get_position(),
                                                       get_position(), screen)
                                   : distance(current_boid.get_position(), get_position());
      if (dist < nearest_distance && dist <= attack_range) {
        nearest_distance = dist;
        nearest_prey     = &current_boid;
      }
    }
    return nearest_prey;
  }

  void attack(Swarm& boids)
  {
    const Boid* prey = find_prey(boids);
    if (prey) {
      Vec3 prey_position = prey->get_position();
      if (!toroidal) {
        Vec3 direction_to_prey = (prey_position - get_position()).normalize();
        set_velocity(direction_to_prey * attack_speed);
      } else {
        Vec3 direction_to_prey =
            toroidal_vec_dist(prey_position, get_position(), screen)
                .normalize();
        set_velocity(direction_to_prey * attack_speed);
      }
    }
  }

 public:
  Predator()
      : Boid()
      , attack_range(0)
      , attack_speed(0)
      , screen(Vec3(600, 300, 300))
      , toroidal(0)
      , prefered_height(screen.z / 3)
      , wind(Vec3(0, 0, 0))
  {}

  Predator(double attack_range_, double attack_speed_, Vec3 screen_,
           bool toroidal_, Vec3 wind_)
      : Boid(Vec3(0, 0, 0), Vec3(0, 0, 0))
      , attack_range(attack_range_)
      , attack_speed(attack_speed_)
      , screen(screen_)
      , toroidal(toroidal_)
      , prefered_height(screen.z / 3)
      , wind(wind_)
  {
    if (attack_range <= 0) {
      throw std::invalid_argument("attack_range must be greater than 0");
    }
    if (attack_speed <= 0) {
      throw std::invalid_argument("attack_speed must be greater than 0");
    }
    initialize_predator();
  }

  // copy constructor
  Predator(const Predator& other)
      : Boid(other)
      , attack_range(other.attack_range)
      , attack_speed(other.attack_speed)
      , screen(other.screen)
  {}

  void update_predator(Swarm& boids)
  {
    if (boids.get_cooldown() >= 1000) {
      attack(boids);
    } else {
      update_boid_velocity(boids.keep_height(*this, prefered_height, 100),
                           attack_speed * 0.6);
    }
    update_boid_velocity(Vec3(), attack_speed);
    update_boid(wind,
                wind.norm() + attack_speed); // il vento ha la precedenza sulle
                                             // intenzioni del predator
    boids.border(*this);
  }

  const double& get_attack_range() const
  {
    return attack_range;
  }
  const double& get_attack_speed() const
  {
    return attack_speed;
  }
  const Vec3& get_screen() const
  {
    return screen;
  }

  Predator& operator=(const Predator& other)
  {
    if (this != &other) {
      set_position(other.get_position());
      set_velocity(other.get_velocity());
      attack_range = other.attack_range;
      attack_speed = other.attack_speed;
      screen       = other.screen;
    }
    return *this;
  }
};

#endif