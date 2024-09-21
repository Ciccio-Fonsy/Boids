#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

int main() {
  namespace B = boids;

  int                  t = 0;
  B::GlobalVariables   global_vars;
  B::PredatorVariables predator_vars;
  B::SwarmVariables    swarm_vars;

  B::initializeParameters(global_vars, predator_vars, swarm_vars);

  B::saveStatisticsOnFile("boids_save.txt", global_vars, predator_vars,
                          swarm_vars);

  B::Predator predator(global_vars, predator_vars);

  B::Swarm swarm(global_vars, swarm_vars, predator);

  sf::RenderWindow window_top;
  sf::RenderWindow window_side;
  sf::CircleShape  boid_shape;
  sf::CircleShape  predator_shape;

  B::drawWindows(window_top, window_side);

  B::initializeShapes(static_cast<float>(swarm_vars.wingspan), boid_shape,
                      predator_shape);

  const float target_frame_time = 1.0f / 180.0f;

  while (window_top.isOpen() && window_side.isOpen() && swarm.size() > 1) {
    sf::Clock clock;

    B::handleEvents(window_top);
    B::handleEvents(window_side);

    window_top.clear();
    B::drawBoids(swarm, predator, window_top, 0, boid_shape, predator_shape);
    window_top.display();

    window_side.clear();
    B::drawBoids(swarm, predator, window_side, 1, boid_shape, predator_shape);
    window_side.display();

    B::updateSimulation(predator, swarm, t, "boids_save.txt");

    float frame_time = clock.getElapsedTime().asSeconds();
    if (frame_time < target_frame_time) {
      sf::sleep(sf::seconds(target_frame_time - frame_time));
    }
  }

  return 0;
}
// Fine del codice