#ifndef STATISTICS
#define STATISTICS

#include "vec3.hpp"

#include <cmath>
#include <numeric>
#include <vector>

// vettore distanza toroidale da da b ad a
const vec3 toroidal_vec_dist(const vec3& a, const vec3& b, const vec3& width)
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  double dz = a.z - b.z;

  if (std::abs(dx) > width.x / 2) {
    if (a.x > b.x) {
      dx -= width.x;
    } else {
      dx += width.x;
    }
  }
  if (std::abs(dy) > width.y / 2) {
    if (a.y > b.y) {
      dy -= width.y;
    } else {
      dy += width.y;
    }
  }

  vec3 dist(dx, dy, dz);

  return dist;
}

// norma del precedente
double toroidal_distance(const vec3& a, const vec3& b, const vec3& width)
{
  return toroidal_vec_dist(a, b, width).norm();
}

double distance(const vec3& a, const vec3& b)
{
  return (a - b).norm();
}

double mean(std::vector<double> const& v)
{
  return std::accumulate(v.begin(), v.end(), 0.0)
       / static_cast<double>(v.size());
}

double dev_std(std::vector<double> const& v, double mean)
{
  double sum =
      std::accumulate(v.begin(), v.end(), 0.0, [mean](double a, double b) {
        return a + (b - mean) * (b - mean);
      });
  return std::sqrt(sum / static_cast<double>(v.size()));
}

#endif