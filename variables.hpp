#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include "vec3.hpp"

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
  double height_factor     = 0.0005;
};

struct ConversionFactors {
  static const double speed_k;
  static const double separation_k;
  static const double cohesion_k;
  static const double alignment_k;
  static const double fear_k;
  static const double height_k;
};

struct LimitValues {
  static const int size_lower;
  static const int size_upper;

  static const int wingspan_lower;
  static const int wingspan_upper;

  static const double min_distance_upper;
  static const double sight_distance_upper;

  static const double factors_lower;
  static const double factors_upper;

  static const double speed_lower;
  static const double speed_upper;

  static const double windspeed_lower;
  static const double windspeed_upper;

  static const double attack_range_lower;
  static const double attack_range_upper;
};
} // namespace boids

#endif // VARIABLES_HPP