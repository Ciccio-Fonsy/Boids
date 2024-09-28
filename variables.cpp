#include "variables.hpp"

#include "vec3.hpp"

namespace boids {
const double ConversionFactors::speed_k      = 100;
const double ConversionFactors::separation_k = 1000;
const double ConversionFactors::cohesion_k   = 1000000;
const double ConversionFactors::alignment_k  = 10000;
const double ConversionFactors::fear_k       = 1000;
const double ConversionFactors::height_k     = 100000;

const int    LimitValues::size_lower           = 1;
const int    LimitValues::size_upper           = 150;
const int    LimitValues::wingspan_lower       = 1;
const int    LimitValues::wingspan_upper       = 5;
const double LimitValues::min_distance_upper   = 100;
const double LimitValues::sight_distance_upper = 500;
const double LimitValues::factors_lower        = 0.001;
const double LimitValues::factors_upper        = 100;
const double LimitValues::speed_lower          = 0.001;
const double LimitValues::speed_upper          = 200;
const double LimitValues::windspeed_lower      = 0.001;
const double LimitValues::windspeed_upper      = 20;
const double LimitValues::attack_range_lower   = 50;
const double LimitValues::attack_range_upper   = 500;
} // namespace boids