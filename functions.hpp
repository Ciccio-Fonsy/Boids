#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "boid.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

namespace boids {
void casualParameters(PredatorVariables& predator_vars,
                      SwarmVariables& swarm_vars, double& windspeed);
void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars);
void drawWindows(sf::RenderWindow& window_xy, sf::RenderWindow& window_xz);
void initializeShapes(double wingspan, sf::CircleShape& boid_shape,
                      sf::CircleShape& predator_shape);
void drawBoids(const Swarm& swarm, const Predator& predator,
               sf::RenderWindow& window, int plane, sf::CircleShape& boid_shape,
               sf::CircleShape& predator_shape);
void printStatistics(Swarm& swarm, int t, const std::string& filename);
void saveStatisticsOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars);

inline void handleEvents(sf::RenderWindow& window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) { window.close(); }
  }
}

inline void updateSimulation(Predator& predator, Swarm& swarm, int& t,
                             int print_period, const std::string& filename) {
  predator.updatePredator(swarm);
  swarm.updateSwarm();

  if (t % print_period == 0) { printStatistics(swarm, t, filename); }
  ++t;
}
} // namespace boids

#endif // FUNCTIONS_HPP