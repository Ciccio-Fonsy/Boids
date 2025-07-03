#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "boid.hpp"
#include "predator.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

namespace boids {
void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars);
void drawWindows(sf::RenderWindow& window_xy, sf::RenderWindow& window_xz);
void initializeShapes(double wingspan, sf::CircleShape& boid_shape,
                      sf::CircleShape& predator_shape);
void handleEvents(sf::RenderWindow& window);
void drawBoids(const Predator* predator, const Swarm& swarm,
               sf::RenderWindow& window, int plane, sf::CircleShape& boid_shape,
               sf::CircleShape& predator_shape);
void saveParametersOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars);
void updateSimulation(Predator* predator, Swarm& swarm, int& t,
                      int print_period, const std::string& filename);

inline double mean(const std::vector<double>& values) {
  if (values.empty()) { throw std::runtime_error("No boids"); }
  return std::accumulate(values.begin(), values.end(), 0.0)
       / static_cast<double>(values.size());
}

inline double stdDev(const std::vector<double>& values) {
  double mean_value = mean(values);
  double sum        = std::accumulate(values.begin(), values.end(), 0.0,
                                      [mean_value](double a, double b) {
                                 return a + (b - mean_value) * (b - mean_value);
                               });
  return std::sqrt(sum / static_cast<double>(values.size()));
}
} // namespace boids

#endif // FUNCTIONS_HPP
