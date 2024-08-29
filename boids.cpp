
#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main()
{
  // finestre usate nella funzione draw e nel main
  sf::RenderWindow windowXY;
  sf::RenderWindow windowXZ;

  draw_windows(windowXY, windowXZ);

  std::vector<boid>::size_type size = 100;
  int wingspan = 2, t = 0;
  double max_speed = 1, min_distance = 30, sight_distance = 150;
  double separation_factor = 0.05, cohesion_factor = 0.00005,
         alignment_factor = 0.005, fear_factor = 0.05;
  const vec3 screen(600, 300, 300);
  vec3 wind(0, 0, 0);
  bool toroidal_bool = false, manually = false;
  double attack_range = 300, attack_speed = 1.1;

  initialize_parameters(manually, size, wingspan, max_speed, min_distance,
                        separation_factor, cohesion_factor, alignment_factor,
                        fear_factor, sight_distance, toroidal_bool, wind,
                        attack_speed, attack_range);

  predator yautja(vec3(0, 0, 0), vec3(0, 0, 0), attack_range, attack_speed,
                  screen, toroidal_bool, wind);

  swarm boids(size, wingspan, max_speed, min_distance, sight_distance,
              separation_factor, cohesion_factor, alignment_factor, fear_factor,
              yautja, screen, toroidal_bool, wind);

  while (windowXY.isOpen() && windowXZ.isOpen() && boids.get_size() > 1) {
    handle_events(windowXY);
    handle_events(windowXZ);

    windowXY.clear();
    draw_boids_on_plane(boids, yautja, windowXY, 0);
    windowXY.display();

    windowXZ.clear();
    draw_boids_on_plane(boids, yautja, windowXZ, 1);
    windowXZ.display();

    update_simulation(yautja, boids, t);
  }

  return 0;
}
// Fine del codice