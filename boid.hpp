#ifndef BOID
#define BOID

#include "vec3.hpp"

class Boid
{
  Vec3 position;
  Vec3 velocity;

 public:
  Boid()
      : position(Vec3(0, 0, 0))
      , velocity(Vec3(0, 0, 0))
  {}
  Boid(Vec3 position_, Vec3 velocity_)
      : position(position_)
      , velocity(velocity_)
  {}

  // Copy constructor
  Boid(const Boid& other)
      : position(other.position)
      , velocity(other.velocity)
  {}

  // restituisce la posizione del boid
  Vec3 get_position() const
  {
    return position;
  }

  // restituisce la velocità del boid
  Vec3 get_velocity() const
  {
    return velocity;
  }

  // imposta la posizione del boid
  void set_position(const Vec3& new_position)
  {
    position = new_position;
  }

  // imposta la velocità del boid
  void set_velocity(const Vec3& new_velocity)
  {
    velocity = new_velocity;
  }

  void update_boid_velocity(Vec3 delta_v, double max_speed)
  {
    velocity += delta_v;

    // Limita la velocità massima
    if (velocity.norm() > max_speed) {
      velocity = velocity.normalize() * max_speed;
    }
  }

  // aggiorna la posizione del boid
  void update_boid(Vec3 delta_v, double max_speed)
  {
    update_boid_velocity(delta_v, max_speed);

    position += velocity;
  }

  // uguaglianza
  bool operator==(const Boid& other) const
  {
    return position == other.position && velocity == other.velocity;
  }

  // disuguaglianza
  bool operator!=(const Boid& other) const
  {
    return !(*this == other);
  }

  // assegna
  Boid operator=(const Boid& other)
  {
    if (this != &other) {
      position = other.position;
      velocity = other.velocity;
    }
    return *this;
  }
};

#endif