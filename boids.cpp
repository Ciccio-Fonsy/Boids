#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main() {
  boids::GlobalVariables   global_vars;
  boids::PredatorVariables predator_vars;
  boids::SwarmVariables    swarm_vars;

  boids::initializeParameters(global_vars, predator_vars, swarm_vars);
  boids::saveStatisticsOnFile("boids_save.txt", global_vars, predator_vars,
                              swarm_vars);

  sf::RenderWindow window_top;
  sf::RenderWindow window_side;
  sf::CircleShape  boid_shape;
  sf::CircleShape  predator_shape;

  boids::drawWindows(window_top, window_side);
  boids::initializeShapes(swarm_vars.wingspan, boid_shape, predator_shape);

  boids::Predator predator(global_vars, predator_vars);
  boids::Swarm    swarm(global_vars, swarm_vars, &predator);

  int t = 0;

  const int   print_period      = 100;
  const float target_frame_time = 1.0f / 180.0f;

  while (window_top.isOpen() && window_side.isOpen() && swarm.size() > 1) {
    sf::Clock clock;

    boids::handleEvents(window_top);
    boids::handleEvents(window_side);

    window_top.clear();
    boids::drawBoids(swarm, predator, window_top, 0, boid_shape,
                     predator_shape);
    window_top.display();

    window_side.clear();
    boids::drawBoids(swarm, predator, window_side, 1, boid_shape,
                     predator_shape);
    window_side.display();

    boids::updateSimulation(predator, swarm, t, print_period, "boids_save.txt");

    float frame_time = clock.getElapsedTime().asSeconds();
    if (frame_time < target_frame_time) {
      sf::sleep(sf::seconds(target_frame_time - frame_time));
    }
  }

  return 0;
}
// Fine del codice