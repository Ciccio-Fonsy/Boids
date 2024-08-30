#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
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

int main()
{
  std::vector<Boid>::size_type size = 100;
  double wingspan                   = 2;
  int t                             = 0;
  double max_speed = 1, min_distance = 30, sight_distance = 150;
  double separation_factor = 0.05, cohesion_factor = 0.00005,
         alignment_factor = 0.005, fear_factor = 0.05;
  const Vec3 screen(600, 300, 300);
  Vec3 wind(0, 0, 0);
  bool toroidal_bool = false, manually = false;
  double attack_range = 300, attack_speed = 1.1;

  initialize_parameters(manually, size, wingspan, max_speed, min_distance,
                        separation_factor, cohesion_factor, alignment_factor,
                        fear_factor, sight_distance, toroidal_bool, wind,
                        attack_speed, attack_range);

  save_statistics_on_file("boids_save.txt", size, wingspan, max_speed,
                          min_distance, separation_factor, cohesion_factor,
                          alignment_factor, fear_factor, sight_distance,
                          attack_speed, attack_range, wind);

  Predator yautja(attack_range, attack_speed, screen, toroidal_bool, wind);

  Swarm boids(size, wingspan, max_speed, min_distance, sight_distance,
              separation_factor, cohesion_factor, alignment_factor, fear_factor,
              yautja, screen, toroidal_bool, wind);

  // finestre usate nella funzione draw e nel main
  sf::RenderWindow windowXY;
  sf::RenderWindow windowXZ;
  sf::CircleShape boid_shape;
  sf::CircleShape predator_shape;

  draw_windows(windowXY, windowXZ);

  initialize_shapes(static_cast<float>(wingspan), boid_shape, predator_shape);

  const float target_frame_time =
      1.0f / 180.0f; // per una durata costante dei cicli

  while (windowXY.isOpen() && windowXZ.isOpen() && boids.get_size() > 1) {
    sf::Clock clock; // Start the clock to measure the time of the current frame

    handle_events(windowXY);
    handle_events(windowXZ);

    windowXY.clear();
    draw_boids_on_plane(boids, yautja, windowXY, 0, boid_shape, predator_shape);
    windowXY.display();

    windowXZ.clear();
    draw_boids_on_plane(boids, yautja, windowXZ, 1, boid_shape, predator_shape);
    windowXZ.display();

    update_simulation(yautja, boids, t, "boids_save.txt");

    float frame_time =
        clock.getElapsedTime()
            .asSeconds(); // Get the time taken by the current frame

    if (frame_time < target_frame_time) {
      sf::sleep(
          sf::seconds(target_frame_time
                      - frame_time)); // Wait to match the target frame time
    }
  }

  return 0;
}
// Fine del codice