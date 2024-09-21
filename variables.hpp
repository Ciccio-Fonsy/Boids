#ifndef VARIABLES_H
#define VARIABLES_H

#include "boid.hpp"
#include "vec3.hpp"

#include <vector>

namespace boids {
struct GlobalVariables {
  Vec3 screen        = Vec3(600, 300, 300);
  Vec3 wind          = Vec3(0, 0, 0);
  bool toroidal_bool = false;
};

struct PredatorVariables {
  double attack_range = 300;
  double attack_speed = 1.1;
};

struct SwarmVariables {
  int size = 100;

  double wingspan       = 2;
  double max_speed      = 1;
  double min_distance   = 30;
  double sight_distance = 150;

  double separation_factor = 0.05;
  double cohesion_factor   = 0.00005;
  double alignment_factor  = 0.005;
  double fear_factor       = 0.05;
};
} // namespace boids

#endif