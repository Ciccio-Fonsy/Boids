#include "functions.hpp"

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
                      SwarmVariables& swarm_vars, double& windspeed) {
  std::random_device rd;
  std::mt19937       gen(rd());

  std::uniform_real_distribution<> dis0(1, 5);
  swarm_vars.wingspan = dis0(gen);

  std::uniform_real_distribution<> dis1(0.001, 2);
  swarm_vars.max_speed = dis1(gen);

  std::uniform_real_distribution<> dis2(swarm_vars.wingspan, 100);
  swarm_vars.min_distance = dis2(gen);

  std::uniform_real_distribution<> dis3(0.001, 100);
  swarm_vars.separation_factor = dis3(gen) / 1000;

  std::uniform_real_distribution<> dis4(0.001, 100);
  swarm_vars.cohesion_factor = dis4(gen) / 1000000;

  std::uniform_real_distribution<> dis5(0.001, 100);
  swarm_vars.alignment_factor = dis5(gen) / 10000;

  std::uniform_real_distribution<> dis6(0.001, 100);
  swarm_vars.fear_factor = dis6(gen) / 1000;

  std::uniform_real_distribution<> dis7(swarm_vars.wingspan, 500);
  swarm_vars.sight_distance = dis7(gen);

  std::uniform_real_distribution<> dis8(0.001, 200);
  predator_vars.attack_speed = dis8(gen) / 100;

  std::uniform_real_distribution<> dis9(50, 500);
  predator_vars.attack_range = dis9(gen);

  std::uniform_real_distribution<> dis10(0.001, 20);
  windspeed = dis10(gen) / 100;
}

void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars) {
  std::string input;
  bool        windbool;
  double      windspeed{0.02};
  bool        manually;

  std::cout << "Enter swarm size (2 ~ 150): ";
  int size;
  std::cin >> size;
  if (size <= 1 || size > 150) {
    std::cout << "This value is not accetable, setted to default value\n";
  } else {
    swarm_vars.size = size;
  }

  std::cout << "Enable wind? [y/N]: ";
  std::cin >> input;
  windbool = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Enable toroidal space? (Recommended with wind enabled) [y/N]: ";
  std::cin >> input;
  global_vars.toroidal_bool =
      (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Insert parameters manually? [y/N]: ";
  std::cin >> input;
  manually = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  if (manually) {
    std::cout << "Enter wingspan (0 ~ 5): ";
    double wingspan;
    std::cin >> wingspan;
    if (wingspan <= 0 || wingspan > 5) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.wingspan = wingspan;
    }

    std::cout << "Enter maximum speed (0 ~ 200): ";
    double max_speed;
    std::cin >> max_speed;
    if (max_speed <= 0 || max_speed > 200) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.max_speed = max_speed / 100;
    }

    std::cout << "Enter minimum distance (0 ~ 100): ";
    double min_distance;
    std::cin >> min_distance;
    if (min_distance <= 0 || min_distance > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.min_distance = min_distance;
    }

    std::cout << "Enter separation factor (0 ~ 100): ";
    double separation_factor;
    std::cin >> separation_factor;
    if (separation_factor <= 0 || separation_factor > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.separation_factor = separation_factor / 1000;
    }

    std::cout << "Enter cohesion factor (0 ~ 100): ";
    double cohesion_factor;
    std::cin >> cohesion_factor;
    if (cohesion_factor <= 0 || cohesion_factor > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.cohesion_factor = cohesion_factor / 1000000;
    }

    std::cout << "Enter alignment factor (0 ~ 100): ";
    double alignment_factor;
    std::cin >> alignment_factor;
    if (alignment_factor <= 0 || alignment_factor > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.alignment_factor = alignment_factor / 10000;
    }

    std::cout << "Enter fear factor (0 ~ 100): ";
    double fear_factor;
    std::cin >> fear_factor;
    if (fear_factor <= 0 || fear_factor > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.fear_factor = fear_factor / 1000;
    }

    std::cout << "Enter sight distance (0 ~ 500): ";
    double sight_distance;
    std::cin >> sight_distance;
    if (sight_distance <= 0 || sight_distance > 500) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.sight_distance = sight_distance;
    }

    std::cout << "Enter predator attack speed (0 ~ 200): ";
    double attack_speed;
    std::cin >> attack_speed;
    if (attack_speed <= 0 || attack_speed > 200) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      predator_vars.attack_speed = attack_speed / 100;
    }

    std::cout << "Enter predator attack range (0 ~ 500): ";
    double attack_range;
    std::cin >> attack_range;
    if (attack_range <= 0 || attack_range > 500) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      predator_vars.attack_range = attack_range;
    }

    if (windbool) {
      std::cout << "Enter wind speed (0 ~ 20): ";
      double windspeed_input;
      std::cin >> windspeed_input;
      if (windspeed_input <= 0 || windspeed_input > 20) {
        std::cout << "This value is not accetable, setted to default value\n";
      } else {
        windspeed = windspeed_input / 100;
      }
    }
  } else {
    bool casual;

    std::cout << "casual parameters geneation? [y/N]: ";
    std::cin >> input;
    casual = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

    if (casual) { casualParameters(predator_vars, swarm_vars, windspeed); }
  }

  std::cout << "Parameters setted at values:\n";
  std::cout << "Size:              " << swarm_vars.size << std::endl;
  std::cout << "Wingspan:          " << swarm_vars.wingspan << std::endl;
  std::cout << "Max speed:         " << swarm_vars.max_speed * 100 << std::endl;
  std::cout << "Min distance:      " << swarm_vars.min_distance << std::endl;
  std::cout << "Separation factor: " << swarm_vars.separation_factor * 1000
            << std::endl;
  std::cout << "Cohesion factor:   " << swarm_vars.cohesion_factor * 1000000
            << std::endl;
  std::cout << "Alignment factor:  " << swarm_vars.alignment_factor * 10000
            << std::endl;
  std::cout << "Fear factor:       " << swarm_vars.fear_factor * 1000
            << std::endl;
  std::cout << "Sight distance:    " << swarm_vars.sight_distance << std::endl;
  std::cout << "Attack speed:      " << predator_vars.attack_speed * 100
            << std::endl;
  std::cout << "Attack range:      " << predator_vars.attack_range << std::endl;

  if (windbool) {
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis_x(-50, 50);
    std::uniform_real_distribution<> dis_y(-50, 50);
    std::uniform_real_distribution<> dis_z(-5, 5);
    global_vars.wind =
        Vec3(dis_x(gen), dis_y(gen), dis_z(gen)).normalize() * windspeed;
    std::cout << "Wind Speed:        " << windspeed * 100 << std::endl;
    std::cout << "Wind:              " << (global_vars.wind * 100).toString()
              << std::endl;
  }
}

void drawWindows(sf::RenderWindow& window_xy, sf::RenderWindow& window_xz) {
  const sf::VideoMode& desktop_mode = sf::VideoMode::getDesktopMode();

  unsigned int screen_width  = desktop_mode.width;
  unsigned int screen_height = desktop_mode.height;

  unsigned int padding        = 12;
  unsigned int app_bar_height = 40;

  unsigned int window_width = (screen_width / 2) - padding;
  unsigned int window_height =
      (screen_height / 2) - 2 * padding - app_bar_height;

  window_xy.create(sf::VideoMode(window_width, window_height), "XY Plane");
  window_xz.create(sf::VideoMode(window_width, window_height), "XZ Plane");

  window_xy.setPosition(
      sf::Vector2i(0, static_cast<int>(window_height + 3 * padding)));
  window_xz.setPosition(sf::Vector2i(0, 0));
}

void initializeShapes(float wingspan, sf::CircleShape& boid_shape,
                      sf::CircleShape& predator_shape) {
  boid_shape.setRadius(wingspan);
  boid_shape.setFillColor(sf::Color::White);

  predator_shape.setRadius(wingspan * 2);
  predator_shape.setFillColor(sf::Color::Red);
}

void saveStatisticsOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars) {
  std::ofstream file(filename, std::ios::app);
  if (file.is_open()) {
    file
        << "\nsize              = " << swarm_vars.size
        << "\nwingspan          = " << swarm_vars.wingspan
        << "\nmax speed         = " << swarm_vars.max_speed * 100
        << "\nmin distance      = " << swarm_vars.min_distance
        << "\nseparation factor = " << swarm_vars.separation_factor * 1000
        << "\ncohesion factor   = " << swarm_vars.cohesion_factor * 1000000
        << "\nalignment factor  = " << swarm_vars.alignment_factor * 10000
        << "\nfear factor       = " << swarm_vars.fear_factor * 1000
        << "\nsight distance    = " << swarm_vars.sight_distance
        << "\nattack speed      = " << predator_vars.attack_speed * 100
        << "\nattack range      = " << predator_vars.attack_range
        << "\nwind speed        = " << global_vars.wind.norm()
        << "\nwind              = " << global_vars.wind.toString()
        << "\ntoroidal          = " << global_vars.toroidal_bool << "\n"
        << "\n     t  mean_distance  distance_std_dev  mean_velocity  velocity_std_dev  n_boids"
        << std::endl;
    file.close();
  }
}
} // namespace boids