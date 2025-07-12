#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "prey.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <memory> //serve per gli unique ptr

// for e algoritmi lambda

int main() {
  boids::GlobalVariables   global_vars;
  boids::PredatorVariables predator_vars;
  boids::SwarmVariables    swarm_vars;

  boids::initializeParameters(global_vars, predator_vars, swarm_vars);
  boids::saveParametersOnFile("boids_save.txt", global_vars, predator_vars,
                              swarm_vars);

  sf::RenderWindow window_top;
  sf::RenderWindow window_side;
  sf::CircleShape  boid_shape;
  sf::CircleShape  predator_shape;

  boids::drawWindows(window_top, window_side);
  boids::initializeShapes(swarm_vars.wingspan, boid_shape, predator_shape);

  std::unique_ptr<boids::Predator> predator = nullptr;

  if (global_vars.predator_bool) {
    predator = std::make_unique<boids::Predator>(global_vars, predator_vars);
  }

  boids::Swarm swarm(global_vars, swarm_vars, predator.get()); //momento clue della simulazione

  int t = 0;

  const int   print_period      = 100;
  const float target_frame_time = 1.0f / 60.0f; //60 frame al secondo

  while (window_top.isOpen() && window_side.isOpen() && swarm.size() > 0) {
    sf::Clock clock;

    boids::handleEvents(window_top);//guardo se è stata chiusa qualche finestra
    boids::handleEvents(window_side);

    window_top.clear(sf::Color(124, 252, 0)); //puliscli la finestra con questo colore
    boids::drawBoids(predator.get(), swarm, window_top, 0, boid_shape, 
                     predator_shape);
    window_top.display();

    window_side.clear(sf::Color(135, 206, 235));
    boids::drawBoids(predator.get(), swarm, window_side, 1, boid_shape, //disegno le finestre una per volta
                     predator_shape);
    window_side.display(); //disegna le modifivhe che hai fatto

    boids::updateSimulation(predator.get(), swarm, t, print_period,
                            "boids_save.txt");

    float frame_time = clock.getElapsedTime().asSeconds(); //tempo che è passato in secondi dall'inizio dell'secuzione di questo clock
    if (frame_time < target_frame_time) {
      sf::sleep(sf::seconds(target_frame_time - frame_time));
    }
  }

  if (swarm.size() == 0) { std::cout << "No boids left, "; }

  std::cout << "terminating execution\nData saved on file boids_save.txt\n";

  return 0;
}
// Fine del codice
