#include "variables.hpp"

#include "vec3.hpp"

namespace boids {
const double ConversionFactors::space_k        = 0.1;
const double ConversionFactors::speed_k        = 6;
const double ConversionFactors::visual_field_k = 360 / M_PI;
const double ConversionFactors::separation_k   = 1000;
const double ConversionFactors::cohesion_k     = 1000000;
const double ConversionFactors::alignment_k    = 10000;
const double ConversionFactors::fear_k         = 1000;
const double ConversionFactors::height_k       = 100000;

const int    LimitValues::size_lower           = 2;
const int    LimitValues::size_upper           = 150;
const double LimitValues::wingspan_lower       = 0.2;
const double LimitValues::wingspan_upper       = 0.5;
const double LimitValues::min_distance_lower   = 3;
const double LimitValues::min_distance_upper   = 10;
const double LimitValues::sight_distance_lower = 10;
const double LimitValues::sight_distance_upper = 50;
const double LimitValues::visual_field_lower   = 100;
const double LimitValues::visual_field_upper   = 360;
const double LimitValues::factors_lower        = 0.001;
const double LimitValues::factors_upper        = 100;
const double LimitValues::speed_lower          = 3;
const double LimitValues::speed_upper          = 10;
const double LimitValues::windspeed_lower      = 0.001;
const double LimitValues::windspeed_upper      = 1;
const double LimitValues::wind_horizontal      = 50;
const double LimitValues::wind_vertical        = 5;
const double LimitValues::attack_range_lower   = 15;
const double LimitValues::attack_range_upper   = 70;
} // namespace boids