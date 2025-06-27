#include "prey.hpp"

#include "boid.hpp"
#include "vec3.hpp"

namespace boids {
Prey::Prey()
    : Boid() {}

Prey::Prey(Vec3 position, Vec3 velocity)
    : Boid(position, velocity) {}
} // namespace boids