#ifndef SWARM
#define SWARM

#include "boid.hpp"
#include "statistics.hpp"
#include "vec3.hpp"

#include <cmath>
#include <random>
#include <vector>

class swarm
{
  std::vector<boid> boids;

  std::vector<boid>::size_type size;
  double wingspan;
  double max_speed;
  double min_distance;
  double sight_distance;
  double separation_factor;
  double cohesion_factor;
  double alignment_factor;
  double fear_factor;
  bool predator;
  boid yautja;
  vec3 screen;
  bool toroidal;
  double prefered_height;
  vec3 wind;
  int cooldown;

  void initialize_positions()
  {
    // Initialize random number generator
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator

    for (auto& b : boids) {
      std::uniform_real_distribution<> ds(0, 1.0); // Define the range

      double x = ds(gen) * screen.x;
      double y = ds(gen) * screen.y;
      double z = ds(gen) * screen.z;

      // Imposta la posizione del boid
      b.set_position(vec3(x, y, z));

      //  genera velocità casuali
      std::uniform_real_distribution<> dv(-1.0, 1.0); // Define the range

      // Calculate the maximum deviation
      double max_deviation = max_speed / std::sqrt(3);

      // Generate random velocities within the desired range
      double vx = dv(gen) * max_deviation;
      double vy = dv(gen) * max_deviation;
      double vz = dv(gen) * max_deviation;

      b.set_velocity(vec3(vx, vy, vz));

      if (b.get_velocity().norm() > max_speed) {
        b.set_velocity(b.get_velocity().normalize() * max_speed);
      }; // Imposta la velocità iniziale del boid
    }
  }

  // REGOLE DI VOLO

  bool is_within_range(const boid& b1, const boid& b2, const double range)
  {
    if (!toroidal) {
      return distance(b1.get_position(), b2.get_position()) <= range;
    } else {
      return toroidal_distance(b1.get_position(), b2.get_position(), screen)
          <= range;
    }
  }

  const vec3
  rule1(const boid& b) // tutti i boid tendono al centro di massa dello stormo
  {
    vec3 perceived_center(0, 0, 0);
    int count{};
    if (!toroidal) { // bordo
      for (std::vector<boid>::size_type i = 0; i < size; i++) {
        if (boids[i] != b && is_within_range(b, boids[i], sight_distance)) {
          // non considero i boid troppo lontani
          perceived_center += boids[i].get_position();
          ++count;
        }
      }
      if (count == 0) {
        return vec3(0, 0, 0);
      }
      perceived_center /= count;
      return ((perceived_center - b.get_position())) * cohesion_factor;
    } else { // toroidale
      for (int j = 0; j < 3; ++j) {
        count = 0;
        for (std::vector<boid>::size_type i = 0; i < size; ++i) {
          if (boids[i] != b && is_within_range(b, boids[i], sight_distance)) {
            if (std::abs(boids[i].get_position()[j] - perceived_center[j])
                < screen[j] / 2) {
              perceived_center[j] =
                  (count * perceived_center[j] + boids[i].get_position()[j])
                  / (count + 1);
            } else {
              perceived_center[j] = (count * perceived_center[j]
                                     + boids[i].get_position()[j] + screen[j])
                                  / (count + 1);
              if (perceived_center[j] > screen[j]) {
                perceived_center[j] -= screen[j];
              }
            }
            ++count;
          }
        }
      }
      return toroidal_vec_dist(perceived_center, b.get_position(), screen)
           * cohesion_factor;
    }
  }

  const vec3
  rule2(boid& b) // tutti i boid si tengono a distanza minima tra loro,
  {
    vec3 c(0, 0, 0);
    if (!toroidal) { // bordo
      for (std::vector<boid>::size_type i = 0; i < size; i++) {
        if (boids[i] != b && is_within_range(boids[i], b, min_distance)) {
          if (distance(boids[i].get_position(), b.get_position()) == 0) {
            b.set_position(boids[i].get_position() + vec3(2 * wingspan, 0, 0));
          } else if (is_within_range(boids[i], b, 2 * wingspan)) {
            vec3 newposition =
                boids[i].get_position()
                + (b.get_position() - boids[i].get_position()).normalize()
                      * wingspan * 2;
            b.set_position(newposition);
          }
          c -= (boids[i].get_position() - b.get_position()).normalize()
             / (distance(boids[i].get_position(), b.get_position()));
        }
      }
    } else { // toroidale
      for (std::vector<boid>::size_type i = 0; i < size; i++) {
        if (boids[i] != b && is_within_range(boids[i], b, min_distance)) {
          if (distance(boids[i].get_position(), b.get_position()) == 0) {
            b.set_position(boids[i].get_position() + vec3(2 * wingspan, 0, 0));
          } else if (is_within_range(boids[i], b, 2 * wingspan)) {
            vec3 newposition =
                boids[i].get_position()
                + (b.get_position() - boids[i].get_position()).normalize()
                      * wingspan * 2;
            b.set_position(newposition);
          }
          c -= toroidal_vec_dist(boids[i].get_position(), b.get_position(),
                                 screen)
                   .normalize()
             / (toroidal_distance(boids[i].get_position(), b.get_position(),
                                  screen));
        }
      }
    }
    return c * separation_factor;
  }

  const vec3
  rule3(const boid& b) // tutti i boid tendono a muoversi nella stessa direzione
  {
    vec3 pv(0, 0, 0);
    int count = 0;
    if (!toroidal) {
      for (std::vector<boid>::size_type i = 0; i < size; i++) {
        if (boids[i] != b && is_within_range(boids[i], b, min_distance)) {
          pv += boids[i].get_velocity();
          ++count;
        }
      }
    }
    if (toroidal) {
      for (std::vector<boid>::size_type i = 0; i < size; i++) {
        if (boids[i] != b && is_within_range(boids[i], b, min_distance)) {
          pv += boids[i].get_velocity();
          ++count;
        }
      }
    }
    if (count == 0) {
      return vec3(0, 0, 0);
    }
    pv /= count;
    return (pv - b.get_velocity()) * alignment_factor;
  }

  const vec3 avoid_predator(const boid& b) // evita il predatore
  {
    vec3 evade_vector(0, 0, 0);
    vec3 distance_to_predator(0, 0, 0);
    if (!toroidal) {
      distance_to_predator = b.get_position() - yautja.get_position();
    } else {
      distance_to_predator =
          toroidal_vec_dist(b.get_position(), yautja.get_position(), screen);
    }
    double distance_norm = distance_to_predator.norm();
    if (distance_norm == 0) {
      evade_vector = b.get_velocity() * max_speed;
    } else if (distance_norm <= sight_distance) {
      vec3 flee_direction  = distance_to_predator.normalize();
      double bounce_factor = (sight_distance - distance_norm) / sight_distance;
      evade_vector         = flee_direction * max_speed * bounce_factor;
    }
    return evade_vector * fear_factor;
  }

  void bounce(boid& b) // rimbalzo
  {
    vec3 bounce       = b.get_velocity();
    vec3 new_position = b.get_position();
    for (int j = 0; j < 3; j++) {
      if (b.get_position()[j] < 0 && b.get_velocity()[j] < 0) {
        bounce[j] *= -0.9;
      } else if (b.get_position()[j] > screen[j] && b.get_velocity()[j] > 0) {
        bounce[j] *= -0.9;
      }
      if (b.get_position()[j] < 0) {
        new_position[j] = 0;
      } else if (b.get_position()[j] > screen[j]) {
        new_position[j] = screen[j];
      }
    }
    b.set_velocity(bounce);
    b.set_position(new_position);
  }

  void teleport(boid& b) // teletrasporto spazio toroidale
  {
    vec3 bounce       = b.get_velocity();
    vec3 new_position = b.get_position();

    for (int j = 0; j < 2; ++j) {
      if (new_position[j] > screen[j]) {
        new_position[j] = new_position[j] - screen[j];
      } else if (new_position[j] < 0) {
        new_position[j] = screen[j] - new_position[j];
      }
    }
    if (b.get_position().z < 0 && b.get_velocity().z < 0) {
      bounce.z *= -0.9;
    } else if (b.get_position().z > screen.z && b.get_velocity().z > 0) {
      bounce.z *= -0.1;
    }
    if (b.get_position().z < 0) {
      new_position.z = 0;
    } else if (b.get_position().z > screen.z) {
      new_position.z = screen.z;
    }
    b.set_velocity(bounce);
    b.set_position(new_position);
  }

 public:
  swarm(std::vector<boid>::size_type size_ = 100, int wingspan_ = 2,
        double max_speed_ = 1, double min_distance_ = 30,
        double sight_distance_ = 150, double separation_factor_ = 0.05,
        double cohesion_factor_ = 0.00005, double alignment_factor_ = 0.005,
        double fear_factor_ = 0.05,
        boid yautja_        = boid(vec3(0, 0, 0), vec3(0, 0, 0)),
        vec3 screen_ = vec3(600, 300, 300), bool toroidal_ = 0,
        vec3 wind_ = vec3(0, 0, 0))
      : size(size_)
      , wingspan(wingspan_)
      , max_speed(max_speed_)
      , min_distance(min_distance_)
      , sight_distance(sight_distance_)
      , separation_factor(separation_factor_)
      , cohesion_factor(cohesion_factor_)
      , alignment_factor(alignment_factor_)
      , fear_factor(fear_factor_)
      , yautja(yautja_)
      , screen(screen_)
      , toroidal(toroidal_)
      , prefered_height(screen_.z * 2 / 3)
      , wind(wind_)
      , cooldown(0)
  {
    if (size <= 1) {
      throw std::invalid_argument("Swarm size must be greater than 1");
    }
    if (wingspan <= 0) {
      throw std::invalid_argument("wingspan must be greater than 0");
    }
    if (max_speed <= 0) {
      throw std::invalid_argument("Maximum speed must be greater than 0");
    }
    if (min_distance <= 0) {
      throw std::invalid_argument("Minimum distance must be greater than 0");
    }
    if (sight_distance <= 0) {
      throw std::invalid_argument("Sight distance must be greater than 0");
    }
    if (alignment_factor <= 0 || alignment_factor > 100) {
      throw std::invalid_argument(
          "Alignment factor must be greater than 0 and smaller than 100");
    }
    if (cohesion_factor <= 0 || cohesion_factor > 100) {
      throw std::invalid_argument(
          "Cohesion factor must be greater than 0 and smaller then 100");
    }
    if (separation_factor <= 0 || separation_factor > 100) {
      throw std::invalid_argument(
          "Separation factor must be greater than 0 and smaller than 100");
    }

    if (fear_factor <= 0 || fear_factor > 100) {
      throw std::invalid_argument(
          "Fear factor must be greater than 0 and smaller than 100");
    }

    for (std::vector<boid>::size_type i = 0; i < size; i++) {
      boids.push_back(boid(
          vec3(0, 0, 0),
          vec3(0, 0, 0))); // Inizialmente posiziona tutti i boids su (0, 0, 0)
    }
    initialize_positions(); // Chiama il metodo per inizializzare le posizioni
  }

  const boid& operator[](std::vector<boid>::size_type i) const
  {
    return boids[i];
  }

  void border(boid& b) // comportamento al bordo
  {
    if (toroidal) {
      teleport(b);
    } else {
      bounce(b);
    }
  }

  const vec3 keep_height(boid& b, double height, double division_factor)
  {
    vec3 comeback(0, 0, 0);
    if (std::abs(b.get_position().z - height) != 0
        && (b.get_position().z - height) * b.get_velocity().z > 0) {
      comeback.z = height - b.get_position().z;
    }
    return comeback / division_factor;
  }

  // Metodo per aggiornare la posizione di tutti i boid nello stormo
  void update_swarm(boid& yautja_)
  {
    ++cooldown;
    yautja = yautja_;
    vec3 v1, v2, v3, v4;

    std::vector<long unsigned int> boids_to_remove;

    // Update velocities and positions for each boid
    for (std::vector<boid>::size_type i = 0; i < size; ++i) {
      boid& current_boid = boids[i];

      if (is_within_range(yautja, current_boid, wingspan)) {
        boids_to_remove.push_back(i); // Check if yautja has reached this boid
        cooldown = 0;
      } else {
        // Calculate new velocities
        v1 = rule1(current_boid);
        v2 = rule2(current_boid);
        v3 = rule3(current_boid);
        v4 = keep_height(current_boid, prefered_height, 1000);

        current_boid.update_boid_velocity(v1 + v2 + v3 + v4, max_speed);
        current_boid.update_boid_velocity(avoid_predator(current_boid),
                                          max_speed);
        current_boid.update_boid(wind, wind.norm() + max_speed);

        border(current_boid);
      }
    }

    // Remove boids in reverse order to avoid invalidating iterators
    for (auto it = boids_to_remove.rbegin(); it != boids_to_remove.rend();
         ++it) {
      boids.erase(boids.begin() + static_cast<int>(*it));
      --size; // Adjust the size of the swarm
    }
  }

  const std::vector<boid>::size_type& get_size() const
  {
    return size;
  }

  const vec3& get_screen() const
  {
    return screen;
  }

  const double& get_max_speed() const
  {
    return max_speed;
  }

  const double& get_min_distance() const
  {
    return min_distance;
  }

  const double& get_separation_factor() const
  {
    return separation_factor;
  }

  const double& get_cohesion_factor() const
  {
    return cohesion_factor;
  }

  const double& get_alignment_factor() const
  {
    return alignment_factor;
  }

  const double& get_fear_factor() const
  {
    return fear_factor;
  }

  const vec3& get_wind() const
  {
    return wind;
  }

  const int& get_wingspan() const
  {
    return wingspan;
  }

  const int& get_cooldown() const
  {
    return cooldown;
  }

  const bool& is_toroidal() const
  {
    return toroidal;
  }
};

#endif
