#ifndef FUNCTIONS
#define FUNCTIONS

#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

namespace boids {
void casualParameters(PredatorVariables& predator_vars,
                      SwarmVariables& swarm_vars, double& windspeed);
void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars);
void drawWindows(sf::RenderWindow& window_xy, sf::RenderWindow& window_xz);
void initializeShapes(float wingspan, sf::CircleShape& boid_shape,
                      sf::CircleShape& predator_shape);
void saveStatisticsOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars);

inline void drawBoids(const Swarm& swarm, const Predator& predator,
                      sf::RenderWindow& window, int plane,
                      sf::CircleShape& boid_shape,
                      sf::CircleShape& predator_shape) {
  if (plane > 1) { throw std::out_of_range("index out of range"); }

  const Vec3& screen = swarm.screen();

  double width  = window.getSize().x;
  double height = window.getSize().y;

  for (int i = 0; i < swarm.size(); ++i) {
    sf::Vector2<double> position;

    switch (plane) {
    case 0:
      position.x = swarm[i].position().x() / screen.x() * width;
      position.y = swarm[i].position().y() / screen.y() * height;
      break;
    case 1:
      position.x = swarm[i].position().x() / screen.x() * width;
      position.y = swarm[i].position().z() / screen.z() * height;
      break;
    }

    boid_shape.setPosition(sf::Vector2f(position));
    window.draw(boid_shape);
  }

  sf::Vector2<double> predator_position;
  switch (plane) {
  case 0:
    predator_position.x = predator.position().x() / screen.x() * width;
    predator_position.y = predator.position().y() / screen.y() * height;
    break;
  case 1:
    predator_position.x = predator.position().x() / screen.x() * width;
    predator_position.y = predator.position().z() / screen.z() * height;
    break;
  }

  predator_shape.setPosition(sf::Vector2f(predator_position));
  window.draw(predator_shape);
}

inline void handleEvents(sf::RenderWindow& window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) { window.close(); }
  }
}

inline void printStatistics(Swarm& swarm, int t, const std::string& filename) {
  std::vector<double> distances, velocities;
  for (int i = 0; i < swarm.size(); ++i) {
    velocities.push_back(swarm[i].velocity().norm() * 100);
    for (int j = i + 1; j < swarm.size(); ++j) {
      distances.push_back(distance(swarm.toroidal(), swarm[i].position(),
                                   swarm[j].position(), swarm.screen()));
    }
  }
  std::ofstream file(filename, std::ios::app);
  double        mean_dist = mean(distances), mean_vel = mean(velocities);
  std::cout << "t = " << std::setw(6) << t
            << "; mean_distance = " << std::setw(7) << mean_dist
            << "; distance_std_dev = " << std::setw(7)
            << stdDev(distances, mean_dist)
            << "; mean_velocity = " << std::setw(7) << mean_vel
            << "; velocity_std_dev = " << std::setw(11)
            << stdDev(velocities, mean_vel) << "; n_boids = " << std::setw(3)
            << swarm.size() << std::endl;
  if (file.is_open()) {
    file << std::setw(6) << t << std::setw(15) << mean_dist << std::setw(18)
         << stdDev(distances, mean_dist) << std::setw(15) << mean_vel
         << std::setw(18) << stdDev(velocities, mean_vel) << std::setw(9)
         << swarm.size() << std::endl;
    file.close();
  } else {
    std::cerr << "Errore: impossibile salvare le posizioni sul file "
              << filename << std::endl;
  }
}

inline void updateSimulation(Predator& predator, Swarm& swarm, int& t,
                             const std::string& filename) {
  predator.updatePredator(swarm);
  swarm.updateSwarm(predator);

  if (t % 100 == 0) { printStatistics(swarm, t, filename); }
  ++t;
}
} // namespace boids

#endif // STATISTICS_H