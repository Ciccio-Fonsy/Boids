#include "functions.hpp"

#include "boid.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <fstream>
#include <iostream>
#include <random>

namespace boids {
void casualParameters(PredatorVariables& predator_vars,
                      SwarmVariables& swarm_vars, double& windspeed) {
  std::random_device rd;
  std::mt19937       gen(rd());

  std::uniform_real_distribution<> dis0(LimitValues::wingspan_lower,
                                        LimitValues::wingspan_upper);
  swarm_vars.wingspan = dis0(gen);

  std::uniform_real_distribution<> dis1(swarm_vars.wingspan,
                                        LimitValues::min_distance_upper);
  swarm_vars.min_distance = dis1(gen);

  std::uniform_real_distribution<> dis2(swarm_vars.min_distance,
                                        LimitValues::sight_distance_upper);
  swarm_vars.sight_distance = dis2(gen);

  std::uniform_real_distribution<> dis3(LimitValues::factors_lower,
                                        LimitValues::factors_upper);
  swarm_vars.separation_factor = dis3(gen) / ConversionFactors::separation_k;
  swarm_vars.cohesion_factor   = dis3(gen) / ConversionFactors::cohesion_k;
  swarm_vars.alignment_factor  = dis3(gen) / ConversionFactors::alignment_k;
  swarm_vars.fear_factor       = dis3(gen) / ConversionFactors::fear_k;
  swarm_vars.height_factor     = dis3(gen) / ConversionFactors::height_k;

  std::uniform_real_distribution<> dis4(LimitValues::speed_lower,
                                        LimitValues::speed_upper);
  swarm_vars.max_speed       = dis4(gen) / ConversionFactors::speed_k;
  predator_vars.attack_speed = dis4(gen) / ConversionFactors::speed_k;

  std::uniform_real_distribution<> dis5(LimitValues::windspeed_lower,
                                        LimitValues::windspeed_upper);
  windspeed = dis5(gen) / ConversionFactors::speed_k;

  std::uniform_real_distribution<> dis6(LimitValues::attack_range_lower,
                                        LimitValues::attack_range_upper);
  predator_vars.attack_range = dis6(gen);
}

void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars) {
  std::string input;
  bool        windbool;
  double      windspeed = 0.02;
  bool        manually;

  std::cout << "Enter swarm size (2 ~ " << LimitValues::size_upper << "): ";
  int size;
  std::cin >> size;
  if (size <= LimitValues::size_lower || size > LimitValues::size_upper) {
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
    std::cout << "Enter wingspan (" << LimitValues::wingspan_lower << " ~ "
              << LimitValues::wingspan_upper << "): ";
    double wingspan;
    std::cin >> wingspan;
    if (wingspan < LimitValues::wingspan_lower
        || wingspan > LimitValues::wingspan_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.wingspan = wingspan;
    }

    std::cout << "Enter maximum speed (" << LimitValues::speed_lower << " ~ "
              << LimitValues::speed_upper << "): ";
    double max_speed;
    std::cin >> max_speed;
    if (max_speed < LimitValues::speed_lower
        || max_speed > LimitValues::speed_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.max_speed = max_speed / ConversionFactors::speed_k;
    }

    std::cout << "Enter minimum distance (" << swarm_vars.wingspan << " ~ "
              << LimitValues::min_distance_upper << "): ";
    double min_distance;
    std::cin >> min_distance;
    if (min_distance < swarm_vars.wingspan
        || min_distance > LimitValues::min_distance_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.min_distance = min_distance;
    }

    std::cout << "Enter separation factor (" << LimitValues::factors_lower
              << " ~ " << LimitValues::factors_upper << "): ";
    double separation_factor;
    std::cin >> separation_factor;
    if (separation_factor < LimitValues::factors_lower
        || separation_factor > LimitValues::factors_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.separation_factor =
          separation_factor / ConversionFactors::separation_k;
    }

    std::cout << "Enter cohesion factor (" << LimitValues::factors_lower
              << " ~ " << LimitValues::factors_upper << "): ";
    double cohesion_factor;
    std::cin >> cohesion_factor;
    if (cohesion_factor < LimitValues::factors_lower
        || cohesion_factor > LimitValues::factors_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.cohesion_factor =
          cohesion_factor / ConversionFactors::cohesion_k;
    }

    std::cout << "Enter alignment factor (" << LimitValues::factors_lower
              << " ~ " << LimitValues::factors_upper << "): ";
    double alignment_factor;
    std::cin >> alignment_factor;
    if (alignment_factor < LimitValues::factors_lower
        || alignment_factor > LimitValues::factors_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.alignment_factor =
          alignment_factor / ConversionFactors::alignment_k;
    }

    std::cout << "Enter fear factor (" << LimitValues::factors_lower << " ~ "
              << LimitValues::factors_upper << "): ";
    double fear_factor;
    std::cin >> fear_factor;
    if (fear_factor < LimitValues::factors_lower
        || fear_factor > LimitValues::factors_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.fear_factor = fear_factor / ConversionFactors::fear_k;
    }

    std::cout << "Enter height factor (" << LimitValues::factors_lower << " ~ "
              << LimitValues::factors_upper << "): ";
    double height_factor;
    std::cin >> height_factor;
    if (height_factor < LimitValues::factors_lower
        || height_factor > LimitValues::factors_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.height_factor = height_factor / ConversionFactors::height_k;
    }

    std::cout << "Enter sight distance (" << swarm_vars.min_distance << " ~ "
              << LimitValues::sight_distance_upper << "): ";
    double sight_distance;
    std::cin >> sight_distance;
    if (sight_distance < swarm_vars.min_distance
        || sight_distance > LimitValues::sight_distance_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      swarm_vars.sight_distance = sight_distance;
    }

    std::cout << "Enter predator attack speed (" << LimitValues::speed_lower
              << " ~ " << LimitValues::speed_upper << "): ";
    double attack_speed;
    std::cin >> attack_speed;
    if (attack_speed < LimitValues::speed_lower
        || attack_speed > LimitValues::speed_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      predator_vars.attack_speed = attack_speed / ConversionFactors::speed_k;
    }

    std::cout << "Enter predator attack range ("
              << LimitValues::attack_range_lower << " ~ "
              << LimitValues::attack_range_upper << "): ";
    double attack_range;
    std::cin >> attack_range;
    if (attack_range <= LimitValues::attack_range_lower
        || attack_range > LimitValues::attack_range_upper) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      predator_vars.attack_range = attack_range;
    }

    if (windbool) {
      std::cout << "Enter wind speed (" << LimitValues::windspeed_lower << " ~ "
                << LimitValues::windspeed_upper << "): ";
      double windspeed_input;
      std::cin >> windspeed_input;
      if (windspeed_input < LimitValues::windspeed_lower
          || windspeed_input > LimitValues::windspeed_upper) {
        std::cout << "This value is not accetable, setted to default value\n";
      } else {
        windspeed = windspeed_input / ConversionFactors::speed_k;
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
  std::cout << "Max speed:         "
            << swarm_vars.max_speed * ConversionFactors::speed_k << std::endl;
  std::cout << "Min distance:      " << swarm_vars.min_distance << std::endl;
  std::cout << "Separation factor: "
            << swarm_vars.separation_factor * ConversionFactors::separation_k
            << std::endl;
  std::cout << "Cohesion factor:   "
            << swarm_vars.cohesion_factor * ConversionFactors::cohesion_k
            << std::endl;
  std::cout << "Alignment factor:  "
            << swarm_vars.alignment_factor * ConversionFactors::alignment_k
            << std::endl;
  std::cout << "Fear factor:       "
            << swarm_vars.fear_factor * ConversionFactors::fear_k << std::endl;
  std::cout << "Height factor:     "
            << swarm_vars.height_factor * ConversionFactors::height_k
            << std::endl;
  std::cout << "Sight distance:    " << swarm_vars.sight_distance << std::endl;
  std::cout << "Attack speed:      "
            << predator_vars.attack_speed * ConversionFactors::speed_k
            << std::endl;
  std::cout << "Attack range:      " << predator_vars.attack_range << std::endl;

  if (windbool) {
    const double wind_horizontal = 50;
    const double wind_vertical   = 5;

    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis_x(-wind_horizontal, wind_horizontal);
    std::uniform_real_distribution<> dis_y(-wind_horizontal, wind_horizontal);
    std::uniform_real_distribution<> dis_z(-wind_vertical, wind_vertical);
    global_vars.wind =
        Vec3(dis_x(gen), dis_y(gen), dis_z(gen)).normalize() * windspeed;
    std::cout << "Wind Speed:        " << windspeed * ConversionFactors::speed_k
              << std::endl;
    std::cout << "Wind:              "
              << (global_vars.wind * ConversionFactors::speed_k).toString()
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

void initializeShapes(double wingspan, sf::CircleShape& boid_shape,
                      sf::CircleShape& predator_shape) {
  float wingspanf = static_cast<float>(wingspan);

  boid_shape.setRadius(wingspanf);
  boid_shape.setFillColor(sf::Color::White);

  predator_shape.setRadius(wingspanf * 2);
  predator_shape.setFillColor(sf::Color::Red);
}

void drawBoids(const Swarm& swarm, const Predator& predator,
               sf::RenderWindow& window, int plane, sf::CircleShape& boid_shape,
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

void saveStatisticsOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars) {
  std::ofstream file(filename, std::ios::app);
  if (file.is_open()) {
    file
        << "\nsize              = " << swarm_vars.size
        << "\nwingspan          = " << swarm_vars.wingspan
        << "\nmax speed         = "
        << swarm_vars.max_speed * ConversionFactors::speed_k
        << "\nmin distance      = " << swarm_vars.min_distance
        << "\nseparation factor = "
        << swarm_vars.separation_factor * ConversionFactors::separation_k
        << "\ncohesion factor   = "
        << swarm_vars.cohesion_factor * ConversionFactors::cohesion_k
        << "\nalignment factor  = "
        << swarm_vars.alignment_factor * ConversionFactors::alignment_k
        << "\nfear factor       = "
        << swarm_vars.fear_factor * ConversionFactors::fear_k
        << "\nheight factor     = "
        << swarm_vars.height_factor * ConversionFactors::height_k
        << "\nsight distance    = " << swarm_vars.sight_distance
        << "\nattack speed      = "
        << predator_vars.attack_speed * ConversionFactors::speed_k
        << "\nattack range      = " << predator_vars.attack_range
        << "\nwind speed        = "
        << global_vars.wind.norm() * ConversionFactors::speed_k
        << "\nwind              = "
        << (global_vars.wind * ConversionFactors::speed_k).toString()
        << "\ntoroidal          = " << global_vars.toroidal_bool << "\n"
        << "\n     t  mean_distance  distance_std_dev  mean_velocity  velocity_std_dev  n_boids"
        << std::endl;
    file.close();
  } else {
    std::cerr << "Errore: impossibile salvare le posizioni sul file "
              << filename << std::endl;
  }
}

void printStatistics(Swarm& swarm, int t, const std::string& filename) {
  std::vector<double> distances;
  std::vector<double> velocities;

  for (int i = 0; i < swarm.size(); ++i) {
    velocities.push_back(swarm[i].velocity().norm()
                         * ConversionFactors::speed_k);
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
  }
}
} // namespace boids