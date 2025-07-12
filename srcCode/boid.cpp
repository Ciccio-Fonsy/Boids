#include "boid.hpp"

#include "vec3.hpp"

namespace boids {
Boid::Boid()
    : position_()
    , velocity_() {}

Boid::Boid(Vec3 position, Vec3 velocity)
    : position_(position)
    , velocity_(velocity) {}

void Boid::updateBoidVelocity(const Vec3& wind, const Vec3& delta_v, //il valore standard del vento è 0
                                     double max_speed) {
  velocity_ += delta_v;

  if ((velocity_ - wind).norm() > max_speed) {
    velocity_ = (velocity_ - wind).normalize() * max_speed + wind; //considerlo la velocità rispetto al vento
  }
}

void Boid::updateBoid(const Vec3& wind, const Vec3& delta_v,
                             double max_speed) {
  updateBoidVelocity(wind, delta_v, max_speed);
  position_ += velocity_ //set_position(position() + velocity()); //
}

void Boid::border(const Vec3& screen, bool toroidal) {
  Vec3 bounce       = velocity_;
  Vec3 new_position = position_;

  for (int j = 0; j < 2; ++j) {
    if (toroidal) {
      if (new_position[j] < 0) { //si trova a sinistra o in alto
        new_position[j] = screen[j] + new_position[j]; //me lo riporta a destra o in basso
      } else if (new_position[j] > screen[j]) {
        new_position[j] -= screen[j];
      }
    } else {
      if (new_position[j] < 0) { //oltre al muro di sinistra
        new_position[j] *= -0.5; //specchiata rispetto al bordo e ridotta di una fattore 0.5 per simulare un rimpazo anelastico
        if (bounce[j] < 0) { bounce[j] *= -0.5; } // per evitare eccezioni in cui mi stavo già allontanando dal bordo
      } else if (new_position[j] > screen[j]) {
        new_position[j] = 1.5 * screen[j] - 0.5 * new_position[j]; // screen-1/2(pos-screen)
        if (bounce[j] > 0) { bounce[j] *= -0.5; }
      }
    }
  }
//la com'onente z non è mai toroidale e per terra è più aneastico
  if (new_position[2] < 0) {
    new_position[2] *= -0.5;
    if (bounce[2] < 0) { bounce[2] *= -0.5; }
  } else if (new_position[2] > screen[2]) { //si trova sotto lo 0 è su
    new_position[2] = 1.5 * screen[2] - 0.5 * new_position[2];
    if (bounce[2] > 0) { bounce[2] *= -0.1; }
  }

  velocity_ = bounce;
  position_ = new_position;
}
} // namespace boids
