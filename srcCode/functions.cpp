#include "functions.hpp"

#include "boid.hpp"
#include "predator.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <random>

namespace boids {
static bool isYes(const std::string& prompt) {
  std::string input;

  std::cout << prompt << " [y/N]: ";
  std::cin >> input;

  std::transform(input.begin(), input.end(), input.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return input == "y" || input == "ye" || input == "yes";
}

template<typename T> static T getParameterFromUser(const std::string& parameter,
                                                   T lower, T upper,
                                                   T conversion_factor = 1) {
  T value;
  std::cout
      << "Enter "
      << parameter
      << " ("
      << lower
      << " ~ "
      << upper
      << "): ";
  std::cin >> value;
  if (value < lower || value > upper) {
    throw std::out_of_range("This value is not acceptable");
  } else {
    return value / conversion_factor;
  }
}

static void casualParameters(GlobalVariables&   global_vars,
                             PredatorVariables& predator_vars,
                             SwarmVariables&    swarm_vars) {
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

  std::uniform_real_distribution<> dis3(LimitValues::visual_field_lower,
                                        LimitValues::visual_field_upper);
  swarm_vars.visual_field = dis3(gen) / ConversionFactors::visual_field_k;

  std::uniform_real_distribution<> dis4(LimitValues::factors_lower,
                                        LimitValues::factors_upper);
  swarm_vars.separation_factor = dis4(gen) / ConversionFactors::separation_k;
  swarm_vars.cohesion_factor   = dis4(gen) / ConversionFactors::cohesion_k;
  swarm_vars.alignment_factor  = dis4(gen) / ConversionFactors::alignment_k;
  swarm_vars.fear_factor       = dis4(gen) / ConversionFactors::fear_k;
  swarm_vars.height_factor     = dis4(gen) / ConversionFactors::height_k;

  if (global_vars.wind_bool) {
    std::uniform_real_distribution<> dis5(LimitValues::windspeed_lower,
                                          LimitValues::windspeed_upper);
    global_vars.windspeed = dis5(gen) / ConversionFactors::speed_k;
  }

  std::uniform_real_distribution<> dis6(LimitValues::speed_lower,
                                        LimitValues::speed_upper);
  swarm_vars.max_speed = dis6(gen) / ConversionFactors::speed_k;

  if (global_vars.predator_bool) {
    predator_vars.attack_speed = dis6(gen) / ConversionFactors::speed_k;

    std::uniform_real_distribution<> dis7(LimitValues::attack_range_lower,
                                          LimitValues::attack_range_upper);
    predator_vars.attack_range = dis7(gen);
  }
}

void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars) {
  swarm_vars.size = getParameterFromUser("swarm size", LimitValues::size_lower,
                                         LimitValues::size_upper);

  global_vars.wind_bool = isYes("Enable wind?");

  global_vars.predator_bool = isYes("Enable predator?");

  global_vars.toroidal_bool =
      isYes("Enable toroidal space? (Recommended with wind enabled)");

  if (isYes("Insert parameters manually?")) {
    swarm_vars.wingspan = getParameterFromUser(
        "wingspan", LimitValues::wingspan_lower, LimitValues::wingspan_upper);

    swarm_vars.max_speed = getParameterFromUser(
        "maximum speed", LimitValues::speed_lower, LimitValues::speed_upper,
        ConversionFactors::speed_k);

    swarm_vars.min_distance =
        getParameterFromUser("minimum distance", swarm_vars.wingspan,
                             LimitValues::min_distance_upper);

    swarm_vars.separation_factor = getParameterFromUser(
        "separation factor", LimitValues::factors_lower,
        LimitValues::factors_upper, ConversionFactors::separation_k);

    swarm_vars.cohesion_factor = getParameterFromUser(
        "cohesion factor", LimitValues::factors_lower,
        LimitValues::factors_upper, ConversionFactors::cohesion_k);

    swarm_vars.alignment_factor = getParameterFromUser(
        "alignment factor", LimitValues::factors_lower,
        LimitValues::factors_upper, ConversionFactors::alignment_k);

    swarm_vars.fear_factor = getParameterFromUser(
        "Fear factor", LimitValues::factors_lower, LimitValues::factors_upper,
        ConversionFactors::fear_k);

    swarm_vars.height_factor = getParameterFromUser(
        "height factor", LimitValues::factors_lower, LimitValues::factors_upper,
        ConversionFactors::height_k);

    swarm_vars.sight_distance =
        getParameterFromUser("sight distance", swarm_vars.min_distance,
                             LimitValues::sight_distance_upper);

    swarm_vars.visual_field = getParameterFromUser(
        "visual field", LimitValues::visual_field_lower,
        LimitValues::visual_field_upper, ConversionFactors::visual_field_k);

    if (global_vars.predator_bool) {
      predator_vars.attack_speed = getParameterFromUser(
          "predator attack speed", LimitValues::speed_lower,
          LimitValues::speed_upper, ConversionFactors::speed_k);

      predator_vars.attack_range = getParameterFromUser(
          "predator attack range", LimitValues::attack_range_lower,
          LimitValues::attack_range_upper);
    }

    if (global_vars.wind_bool) {
      global_vars.windspeed = getParameterFromUser(
          "wind speed", LimitValues::windspeed_lower,
          LimitValues::windspeed_upper, ConversionFactors::speed_k);
    }
  } else {
    if (isYes("Casual parameters geneation?")) {
      casualParameters(global_vars, predator_vars, swarm_vars);
    }
  }

  std::cout << "Parameters set to values:\n";
  std::cout << "Size:              " << swarm_vars.size << std::endl;
  std::cout << "Wingspan:          " << swarm_vars.wingspan << std::endl;
  std::cout
      << "Max speed:         "
      << swarm_vars.max_speed * ConversionFactors::speed_k
      << std::endl;
  std::cout << "Min distance:      " << swarm_vars.min_distance << std::endl;
  std::cout
      << "Separation factor: "
      << swarm_vars.separation_factor * ConversionFactors::separation_k
      << std::endl;
  std::cout
      << "Cohesion factor:   "
      << swarm_vars.cohesion_factor * ConversionFactors::cohesion_k
      << std::endl;
  std::cout
      << "Alignment factor:  "
      << swarm_vars.alignment_factor * ConversionFactors::alignment_k
      << std::endl;
  std::cout
      << "Fear factor:       "
      << swarm_vars.fear_factor * ConversionFactors::fear_k
      << std::endl;
  std::cout
      << "Height factor:     "
      << swarm_vars.height_factor * ConversionFactors::height_k
      << std::endl;
  std::cout << "Sight distance:    " << swarm_vars.sight_distance << std::endl;
  std::cout
      << "Visual field:      "
      << swarm_vars.visual_field * ConversionFactors::visual_field_k
      << std::endl;

  if (global_vars.predator_bool) {
    std::cout
        << "Attack speed:      "
        << predator_vars.attack_speed * ConversionFactors::speed_k
        << std::endl;
    std::cout
        << "Attack range:      "
        << predator_vars.attack_range
        << std::endl;
  }

  if (global_vars.wind_bool) {
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis_x(-LimitValues::wind_horizontal,
                                           LimitValues::wind_horizontal);
    std::uniform_real_distribution<> dis_y(-LimitValues::wind_horizontal,
                                           LimitValues::wind_horizontal);
    std::uniform_real_distribution<> dis_z(-LimitValues::wind_vertical,
                                           LimitValues::wind_vertical);
    global_vars.wind = Vec3(dis_x(gen), dis_y(gen), dis_z(gen)).normalize()
                     * global_vars.windspeed;
    std::cout
        << "Wind Speed:        "
        << global_vars.windspeed * ConversionFactors::speed_k
        << std::endl;
    std::cout
        << "Wind:              "
        << (global_vars.wind * ConversionFactors::speed_k).toString()
        << std::endl;
  }
}

void saveStatisticsOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars) {
  std::ofstream file(filename, std::ios::app);
  if (file.is_open()) {
    file
        << "\nsize              = "
        << swarm_vars.size
        << "\nwingspan          = "
        << swarm_vars.wingspan
        << "\nmax speed         = "
        << swarm_vars.max_speed * ConversionFactors::speed_k
        << "\nmin distance      = "
        << swarm_vars.min_distance
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
        << "\nsight distance    = "
        << swarm_vars.sight_distance
        << "\nvisual field      = "
        << swarm_vars.visual_field * ConversionFactors::visual_field_k
        << "\npredator          = "
        << global_vars.predator_bool;
    if (global_vars.predator_bool) {
      file
          << "\nattack speed      = "
          << predator_vars.attack_speed * ConversionFactors::speed_k
          << "\nattack range      = "
          << predator_vars.attack_range;
    }
    file << "\nwind              = " << global_vars.wind_bool;
    if (global_vars.wind_bool) {
      file
          << "\nwind speed        = "
          << global_vars.windspeed * ConversionFactors::speed_k
          << "\nwind vector       = "
          << (global_vars.wind * ConversionFactors::speed_k).toString();
    }
    file
        << "\ntoroidal          = "
        << global_vars.toroidal_bool
        << "\n\n     t  mean_distance  distance_std_dev  mean_velocity  velocity_std_dev  n_boids"
        << std::endl;
    file.close();
  } else {
    throw std::runtime_error("Error: unable to save data to file: " + filename);
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
  boid_shape.setFillColor(sf::Color::Black);

  predator_shape.setRadius(wingspanf * 2);
  predator_shape.setFillColor(sf::Color::Red);
}

void handleEvents(sf::RenderWindow& window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
      std::cout << "Program closed, ";
    }
  }
}

void drawBoids(const Predator* predator, const Swarm& swarm,
               sf::RenderWindow& window, int plane, sf::CircleShape& boid_shape,
               sf::CircleShape& predator_shape) {
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
    default: throw std::out_of_range("index out of range");
    }

    boid_shape.setPosition(sf::Vector2f(position));
    window.draw(boid_shape);
  }

  if (predator) {
    sf::Vector2<double> predator_position;
    switch (plane) {
    case 0:
      predator_position.x = predator->position().x() / screen.x() * width;
      predator_position.y = predator->position().y() / screen.y() * height;
      break;
    case 1:
      predator_position.x = predator->position().x() / screen.x() * width;
      predator_position.y = predator->position().z() / screen.z() * height;
      break;
    default: throw std::out_of_range("index out of range");
    }

    predator_shape.setPosition(sf::Vector2f(predator_position));
    window.draw(predator_shape);
  }
}

static void printStatistics(const Swarm& swarm, int t,
                            const std::string& filename) {
  std::vector<double> distances;
  std::vector<double> velocities;

  for (int i = 0; i < swarm.size(); ++i) {
    velocities.push_back(swarm[i].velocity().norm()
                         * ConversionFactors::speed_k);
    for (int j = i + 1; j < swarm.size(); ++j) {
      distances.push_back(swarm[i].position().distance(
          swarm.toroidal(), swarm[j].position(), swarm.screen()));
    }
  }

  if (distances.empty()) { distances.push_back(0); }

  std::ofstream file(filename, std::ios::app);

  double mean_dist = mean(distances);
  double mean_vel  = mean(velocities);
  double std_dist  = stdDev(distances);
  double std_vel   = stdDev(velocities);

  if (file.is_open()) {
    file
        << std::setw(6)
        << t
        << std::setw(15)
        << mean_dist
        << std::setw(18)
        << std_dist
        << std::setw(15)
        << mean_vel
        << std::setw(18)
        << std_vel
        << std::setw(9)
        << swarm.size()
        << std::endl;
    file.close();
  }
}

void updateSimulation(Predator* predator, Swarm& swarm, int& t,
                      int print_period, const std::string& filename) {
  if (predator) { predator->updatePredator(swarm); }
  swarm.updateSwarm();

  if (t % print_period == 0) { printStatistics(swarm, t, filename); }
  ++t;
}
} // namespace boids