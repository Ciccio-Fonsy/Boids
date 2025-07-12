#include "swarm.hpp"

#include "boid.hpp"
#include "prey.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <algorithm>
#include <cassert>
#include <random>
#include <stdexcept>
#include <vector>

namespace boids {
void Swarm::init() {
  std::random_device               rd;
  std::mt19937                     gen(rd());
  std::uniform_real_distribution<> ds(0, 1.0);
  std::uniform_real_distribution<> dv(-1.0, 1.0);

  for (Prey& b : preys_) {
    const double x = ds(gen) * screen_.x_;
    const double y = ds(gen) * screen_.y_;

    b.set_position(Vec3(x, y, screen_.z_));

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

void Swarm::bounce(Prey& b) {
  const Vec3& b_position = b.position();
  const Vec3& b_velocity = b.velocity();

  for (Prey& other_prey : preys_) {
    if (other_prey != b && isWithinRange(other_prey, b, 2 * wingspan_)) { 
      const Vec3& other_position = other_prey.position();
      const Vec3& other_velocity = other_prey.velocity();

      if (b_position == other_position) {
        b.set_position(b_position + Vec3(0, 0, wingspan_ / 10)); //se sono sovrapposti i spodto di poco per far continuare il programma
      }

      const Vec3 separation_vector =
          other_position.vecDistance(toroidal_, b_position, screen_); //distanza tra i pray
      const Vec3 relative_velocity = b_velocity - other_velocity;
      const bool approaching = separation_vector.dot(relative_velocity) > 0; //e il prodotto scalare, e quindi il coseno, è positivo: si stanno avvicinando

      if (approaching) {
        const Vec3 collision_normal = separation_vector.normalize(); //versore separazione boids
        const Vec3 velocity_b_after =
            b_velocity
            - collision_normal * (relative_velocity.dot(collision_normal)); //componente della velocità lungo la normale di collisione, urto totalmente anelstico
        const Vec3 velocity_other_after =
            other_velocity
            + collision_normal * (relative_velocity.dot(collision_normal)); //uguale per other, è anelastico perchè è coinvolto anche il separation factor

        b.set_velocity(velocity_b_after * 0.1); //rallentano molto, storditi
        other_prey.set_velocity(velocity_other_after * 0.1);
      }
    }
  }
}

Vec3 Swarm::separation(const Prey& b) const {
  const Vec3& b_position = b.position();
  Vec3        c; //vettore su cui sto lavorando

  for (const Prey& other_prey : preys_) {
    const Vec3& other_position = other_prey.position();

    if (other_prey != b
        && isWithinRange(b, other_prey, min_distance_)
        && other_position != b_position //è sempre diversa perchè update swarm prima chiama bounce
        && isWithinField(b, other_prey)) {
      const Vec3 separation =
          other_position.vecDistance(toroidal_, b_position, screen_);
      c -= separation.normalize() / separation.norm(); //somma di le direzioni di spearazione con modulo invarsamente proporzionale alla norma
    } //più evidente se ci sono tanti boids in una direzione
  }

  return c * separation_factor_;
}

Vec3 Swarm::cohesion(const Prey& b) const {
  Vec3 perceived_center;
  int  count = 0;

  for (const Prey& other_prey : preys_) {
    if (other_prey != b
        && isWithinRange(b, other_prey, sight_distance_)
        && isWithinField(b, other_prey)) {
      Vec3 position = other_prey.position();

      if (toroidal_) {
        for (int j = 0; j < 2; ++j) {
          const double pos_diff = position[j] - perceived_center[j];

          if (std::abs(pos_diff) >= screen_[j] / 2) {
            position[j] += (pos_diff < 0 ? screen_[j] : -screen_[j]);
          }

          perceived_center[j] =
              (count * perceived_center[j] + position[j]) / (count + 1); //media pesata rispetto a quanti boids si riferisce il percived center(devo sapere sempre dov'è CM) 
          perceived_center[j] =
              std::fmod(perceived_center[j], screen_[j]); //se il centro di massa è fuori dai limiti me lo sposta dentro (ho tolto un + sceen)
                                                          //(forse potevamo usare altrove, bordo di boid)
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
    perceived_center[2] /= count; //solo sulla z avevo la somma diretta
  } else {
    perceived_center /= count;
  }

  return perceived_center.vecDistance(toroidal_, b.position(), screen_) //non è normalzzata
       * cohesion_factor_;
}

Vec3 Swarm::alignment(const Prey& b) const { //guara tutte le prede nel campo visivo e mi restituisce la media delle differenze di velocità
  Vec3 pv;
  int  count = 0;

  for (const Prey& other_prey : preys_) {
    if (other_prey != b
        && isWithinRange(b, other_prey, sight_distance_)
        && isWithinField(b, other_prey)) {
      pv += other_prey.velocity();
      ++count;
    }
  }

  if (count == 0) { return Vec3(0, 0, 0); }

  pv /= count;
  return (pv - b.velocity()) * alignment_factor_;
}

Vec3 Swarm::height(const Prey& b) const { //entra in gioco solo se si allontana dall'altezza preferita
  Vec3   correction;
  double dh = preferred_height_ - b.position().z_;

  if (dh * b.velocity().z_ <= 0) { correction.z_ = dh; }

  return correction * height_factor_;
}

Vec3 Swarm::fear(const Prey& b) const {
  assert(predator_ != nullptr);
  Vec3       evade_vector;
  const Vec3 distance_to_predator =
      b.position().vecDistance(toroidal_, predator_->position(), screen_);
  const double distance_norm = distance_to_predator.norm();

  if (distance_norm == 0) { //caso che non si verifica
    evade_vector = b.velocity().normalize() * max_speed_;
  } else if (distance_norm <= sight_distance_) { //(is whithin range)
    const Vec3   flee_direction = distance_to_predator.normalize(); //distance from predator
    const double evade_strength =
        (sight_distance_ - distance_norm) / sight_distance_; //percentiale della velocità massima che dipende da quanto sono lontani

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
    , visual_field_(60)
    , preferred_height_(200)
    , separation_factor_(0.05)
    , cohesion_factor_(0.00005)
    , alignment_factor_(0.005)
    , fear_factor_(0.05)
    , height_factor_(0.0005)
    , screen_(Vec3(600, 300, 300))
    , wind_()
    , toroidal_()
    , predator_(nullptr) {
  preys_ = std::vector<Prey>(static_cast<std::size_t>(size_));

  init();
}

Swarm::Swarm(const GlobalVariables& global_vars,
             const SwarmVariables& swarm_vars, Boid* predator)
    : size_(swarm_vars.size)
    , wingspan_(swarm_vars.wingspan)
    , max_speed_(swarm_vars.max_speed)
    , min_distance_(swarm_vars.min_distance)
    , sight_distance_(swarm_vars.sight_distance)
    , visual_field_(swarm_vars.visual_field)
    , preferred_height_(global_vars.screen.z_ * 2 / 3)
    , separation_factor_(swarm_vars.separation_factor)
    , cohesion_factor_(swarm_vars.cohesion_factor)
    , alignment_factor_(swarm_vars.alignment_factor)
    , fear_factor_(swarm_vars.fear_factor)
    , height_factor_(swarm_vars.height_factor)
    , screen_(global_vars.screen)
    , wind_(global_vars.wind)
    , toroidal_(global_vars.toroidal_bool)
    , predator_(predator) {
  if (size_ <= 0) {
    throw std::invalid_argument("Swarm size must be greater than 0");
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

  preys_ = std::vector<Prey>(static_cast<std::size_t>(size_));

  init();
}

void Swarm::updateSwarm() {
  preys_.erase(
      std::remove_if( //mi sposta tutti gli elementi eliminabili alla fine del vec e mi passa un itaratore al primo elemento eliminabile
          preys_.begin(), preys_.end(),
          [this](Prey& prey) { //lamda prende lo stormo per reference e o modifica
            if (predator_ && isWithinRange(*predator_, prey, wingspan_)) {
              predator_->resetCooldown();
              return true; // Mark this prey for removal
            } else { //se non la devo eliminare aggiorno la sua posizione secondo i criteri
              const Vec3 v1 = separation(prey);
              const Vec3 v2 = cohesion(prey);
              const Vec3 v3 = alignment(prey);
              const Vec3 v4 = height(prey);

              bounce(prey);
              prey.updateBoidVelocity(wind_, v1 + v2 + v3 + v4, max_speed_);

              if (predator_) {
                prey.updateBoidVelocity(wind_, fear(prey), max_speed_);
              }

              prey.stall(wind_, max_speed_);
              prey.updateBoid(wind_, wind_, max_speed_);
              prey.border(screen_, toroidal_);

              return false; // Keep this prey
            }
          }),
      preys_.end());

  size_ = static_cast<int>(preys_.size()); //aggiusto la dim del vettore
}
} // namespace boids
