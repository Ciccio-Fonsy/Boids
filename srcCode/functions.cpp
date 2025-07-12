#include "functions.hpp"

#include "boid.hpp"
#include "predator.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
//#include <cctype> inutile
#include <fstream> //file
#include <iomanip> //gestisce la formattazione del file
#include <iostream>
#include <random>

namespace boids {
static bool isYes(const std::string& prompt) { //viene vista solo all'interno di questa translation unit
  std::string input;

  std::cout << prompt << " [y/N]: ";
  std::cin >> input;

  std::transform(input.begin(), input.end(), input.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return input == "y" || input == "ye" || input == "yes";
}

template<typename T>
static T getParameterFromUser(const std::string& parameter, T lower, T upper,
                              const std::string& measure_unit,
                              T conversion_factor, bool factor = 0) {
  T value;
  std::cout
      << "Enter "
      << parameter
      << " ("
      << lower
      << " ~ "
      << upper
      << ") "
      << measure_unit
      << ": ";
  std::cin >> value;
  if (value < lower || value > upper) {
    throw std::out_of_range("This value is not acceptable");
  } else if (factor) { //me lo riporta ad una valore vicino al valore medio
    return (value / 5 + 40) / conversion_factor;
  } else {
    return value / conversion_factor;
  }
}

static void casualBoidsParameters(GlobalVariables&   global_vars,
                                  PredatorVariables& predator_vars,
                                  SwarmVariables&    swarm_vars) {
  std::random_device rd;
  std::mt19937       gen(rd());

  std::uniform_real_distribution<> dis0(LimitValues::wingspan_lower,
                                        LimitValues::wingspan_upper);
  swarm_vars.wingspan = dis0(gen) / ConversionFactors::space_k;

  std::uniform_real_distribution<> dis1(LimitValues::min_distance_lower,
                                        LimitValues::min_distance_upper);
  swarm_vars.min_distance = dis1(gen) / ConversionFactors::space_k;

  std::uniform_real_distribution<> dis2(LimitValues::sight_distance_lower,
                                        LimitValues::sight_distance_upper);
  swarm_vars.sight_distance = dis2(gen) / ConversionFactors::space_k;

  std::uniform_real_distribution<> dis3(LimitValues::visual_field_lower,
                                        LimitValues::visual_field_upper);
  swarm_vars.visual_field = dis3(gen) / ConversionFactors::visual_field_k;

  std::uniform_real_distribution<> dis6(LimitValues::speed_lower,
                                        LimitValues::speed_upper);
  swarm_vars.max_speed = dis6(gen) / ConversionFactors::speed_k;

  if (global_vars.predator_bool) {
    std::uniform_real_distribution<> dis7(swarm_vars.max_speed
                                              * ConversionFactors::speed_k,
                                          LimitValues::speed_upper);
    predator_vars.attack_speed = dis7(gen) / ConversionFactors::speed_k;

    std::uniform_real_distribution<> dis8(LimitValues::attack_range_lower,
                                          LimitValues::attack_range_upper);
    predator_vars.attack_range = dis8(gen) / ConversionFactors::space_k;
  }

  if (global_vars.wind_bool) {
    std::uniform_real_distribution<> dis5(LimitValues::windspeed_lower,
                                          LimitValues::windspeed_upper);
    global_vars.windspeed = dis5(gen) / ConversionFactors::speed_k;
  }
}

static void casualSimulationParameters(SwarmVariables& swarm_vars) {
  std::random_device rd;
  std::mt19937       gen(rd());

  std::uniform_real_distribution<> dis4(LimitValues::factors_lower,
                                        LimitValues::factors_upper);
  swarm_vars.separation_factor =
      (dis4(gen) / 5 + 40) / ConversionFactors::separation_k;
  swarm_vars.cohesion_factor =
      (dis4(gen) / 5 + 40) / ConversionFactors::cohesion_k;
  swarm_vars.alignment_factor =
      (dis4(gen) / 5 + 40) / ConversionFactors::alignment_k;
  swarm_vars.fear_factor   = (dis4(gen) / 5 + 40) / ConversionFactors::fear_k;
  swarm_vars.height_factor = (dis4(gen) / 5 + 40) / ConversionFactors::height_k;
}

void initializeParameters(GlobalVariables&   global_vars,
                          PredatorVariables& predator_vars,
                          SwarmVariables&    swarm_vars) {
  swarm_vars.size = getParameterFromUser("swarm size", LimitValues::size_lower,
                                         LimitValues::size_upper, "boids", 1);

  global_vars.wind_bool = isYes("Enable wind?");

  global_vars.predator_bool = isYes("Enable predator?");

  global_vars.toroidal_bool =
      isYes("Enable toroidal space? (Recommended with wind enabled)");

  if (isYes("Insert boid parameters manually?")) {
    swarm_vars.wingspan = getParameterFromUser(
        "wingspan", LimitValues::wingspan_lower, LimitValues::wingspan_upper,
        "m", ConversionFactors::space_k);

    swarm_vars.min_distance = getParameterFromUser(
        "minimum distance", LimitValues::min_distance_lower,
        LimitValues::min_distance_upper, "m", ConversionFactors::space_k);

    swarm_vars.sight_distance = getParameterFromUser(
        "sight distance", LimitValues::sight_distance_lower,
        LimitValues::sight_distance_upper, "m", ConversionFactors::space_k);

    swarm_vars.visual_field =
        getParameterFromUser("visual field", LimitValues::visual_field_lower,
                             LimitValues::visual_field_upper, "°",
                             ConversionFactors::visual_field_k);

    swarm_vars.max_speed = getParameterFromUser(
        "maximum speed", LimitValues::speed_lower, LimitValues::speed_upper,
        "m/s", ConversionFactors::speed_k);

    if (global_vars.predator_bool) {
      predator_vars.attack_range = getParameterFromUser(
          "predator attack range", LimitValues::attack_range_lower,
          LimitValues::attack_range_upper, "m", ConversionFactors::space_k);

      predator_vars.attack_speed = getParameterFromUser(
          "predator attack speed",
          swarm_vars.max_speed * ConversionFactors::speed_k,
          LimitValues::speed_upper, "m/s", ConversionFactors::speed_k);
    }

    if (global_vars.wind_bool) {
      global_vars.windspeed = getParameterFromUser(
          "wind speed", LimitValues::windspeed_lower,
          LimitValues::windspeed_upper, "m/s", ConversionFactors::speed_k);
    }
  } else {
    if (isYes("Casual boids parameters generation?")) {
      casualBoidsParameters(global_vars, predator_vars, swarm_vars);
    }
  }
  if (isYes("Insert simulation parameters manually?")) {
    swarm_vars.separation_factor = getParameterFromUser(
        "separation factor", LimitValues::factors_lower,
        LimitValues::factors_upper, "", ConversionFactors::separation_k, 1);

    swarm_vars.cohesion_factor = getParameterFromUser(
        "cohesion factor", LimitValues::factors_lower,
        LimitValues::factors_upper, "", ConversionFactors::cohesion_k, 1);

    swarm_vars.alignment_factor = getParameterFromUser(
        "alignment factor", LimitValues::factors_lower,
        LimitValues::factors_upper, "", ConversionFactors::alignment_k, 1);

    swarm_vars.height_factor = getParameterFromUser(
        "height factor", LimitValues::factors_lower, LimitValues::factors_upper,
        "", ConversionFactors::height_k, 1);

    if (global_vars.predator_bool) {
      swarm_vars.fear_factor = getParameterFromUser(
          "Fear factor", LimitValues::factors_lower, LimitValues::factors_upper,
          "", ConversionFactors::fear_k, 1);
    }
  } else {
    if (isYes("Casual parameters geneation?")) {
      casualSimulationParameters(swarm_vars);
    }
  }

  std::cout << "Parameters set to values:\n";
  std::cout
      << "Size:              "
      << swarm_vars.size
      << " boids"
      << std::endl;
  std::cout
      << "Wingspan:          "
      << swarm_vars.wingspan * ConversionFactors::space_k
      << " m"
      << std::endl;
  std::cout
      << "Min distance:      "
      << swarm_vars.min_distance * ConversionFactors::space_k
      << " m"
      << std::endl;
  std::cout
      << "Sight distance:    "
      << swarm_vars.sight_distance * ConversionFactors::space_k
      << " m"
      << std::endl;
  std::cout
      << "Visual field:      "
      << swarm_vars.visual_field * ConversionFactors::visual_field_k
      << " °"
      << std::endl;
  std::cout
      << "Max speed:         "
      << swarm_vars.max_speed * ConversionFactors::speed_k
      << " m/s"
      << std::endl;
  std::cout
      << "Separation factor: "
      << (swarm_vars.separation_factor * ConversionFactors::separation_k - 40)
             * 5
      << std::endl;
  std::cout
      << "Cohesion factor:   "
      << (swarm_vars.cohesion_factor * ConversionFactors::cohesion_k - 40) * 5
      << std::endl;
  std::cout
      << "Alignment factor:  "
      << (swarm_vars.alignment_factor * ConversionFactors::alignment_k - 40) * 5
      << std::endl;
  std::cout
      << "Height factor:     "
      << (swarm_vars.height_factor * ConversionFactors::height_k - 40) * 5
      << std::endl;

  if (global_vars.predator_bool) {
    std::cout
        << "Fear factor:       "
        << (swarm_vars.fear_factor * ConversionFactors::fear_k - 40) * 5
        << std::endl;
    std::cout
        << "Attack range:      "
        << predator_vars.attack_range * ConversionFactors::space_k
        << " m"
        << std::endl;
    std::cout
        << "Attack speed:      "
        << predator_vars.attack_speed * ConversionFactors::speed_k
        << " m/s"
        << std::endl;
  }

  if (global_vars.wind_bool) { //generatore casuale direzione del vento
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
        << "Wind speed:        "
        << global_vars.windspeed * ConversionFactors::speed_k
        << " m/s"
        << std::endl;
    std::cout
        << "Wind velocity:     "
        << (global_vars.wind * ConversionFactors::speed_k)
        << " m/s"
        << std::endl;
  }
}

void saveParametersOnFile(const std::string&       filename,
                          const GlobalVariables&   global_vars,
                          const PredatorVariables& predator_vars,
                          const SwarmVariables&    swarm_vars) {
  std::ofstream file(filename, std::ios::app);  //crea uno stream di stampa su file, che si chiama file e che mi va a stampare in in filemane
                                                //(std::ios::app apre in modalitòà append) trunc se volessi eliminarle
  if (file.is_open()) {
    file
        << "\nsize              = "
        << swarm_vars.size
        << " boids"
        << "\nwingspan          = "
        << swarm_vars.wingspan * ConversionFactors::space_k
        << " m"
        << "\nmin distance      = "
        << swarm_vars.min_distance * ConversionFactors::space_k
        << " m"
        << "\nsight distance    = "
        << swarm_vars.sight_distance * ConversionFactors::space_k
        << " m"
        << "\nvisual field      = "
        << swarm_vars.visual_field * ConversionFactors::visual_field_k
        << " °"
        << "\nmax speed         = "
        << swarm_vars.max_speed * ConversionFactors::speed_k
        << " m/s"
        << "\nseparation factor = "
        << (swarm_vars.separation_factor * ConversionFactors::separation_k - 40)
               * 5
        << "\ncohesion factor   = "
        << (swarm_vars.cohesion_factor * ConversionFactors::cohesion_k - 40) * 5
        << "\nalignment factor  = "
        << (swarm_vars.alignment_factor * ConversionFactors::alignment_k - 40)
               * 5
        << "\nheight factor     = "
        << (swarm_vars.height_factor * ConversionFactors::height_k - 40) * 5
        << "\npredator          = "
        << global_vars.predator_bool;
    if (global_vars.predator_bool) {
      file
          << "\nfear factor       = "
          << (swarm_vars.fear_factor * ConversionFactors::fear_k - 40) * 5
          << "\nattack range      = "
          << predator_vars.attack_range * ConversionFactors::space_k
          << " m"
          << "\nattack speed      = "
          << predator_vars.attack_speed * ConversionFactors::speed_k
          << " m/s";
    }
    file << "\nwind              = " << global_vars.wind_bool;
    if (global_vars.wind_bool) {
      file
          << "\nwind speed        = "
          << global_vars.windspeed * ConversionFactors::speed_k
          << " m/s"
          << "\nwind velocity     = "
          << (global_vars.wind * ConversionFactors::speed_k)
          << " m/s";
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

void drawWindows(sf::RenderWindow& window_xy, sf::RenderWindow& window_xz) { //sto passando 2 fineste render window create vuote nel main
  const sf::VideoMode& desktop_mode = sf::VideoMode::getDesktopMode();

  unsigned int screen_width  = desktop_mode.width;
  unsigned int screen_height = desktop_mode.height;

  unsigned int padding        = 12; //
  unsigned int app_bar_height = 40; //tengono conto della barra delle applicazioni e della finestra di linux (misurate da noi)

  unsigned int window_width = (screen_width / 2) - padding;
  unsigned int window_height =
      (screen_height / 2) - 2 * padding - app_bar_height;

  window_xy.create(sf::VideoMode(window_width, window_height), "Top view"); //viene disegnata con queste misure e questo nome
  window_xz.create(sf::VideoMode(window_width, window_height), "Side view");

  window_xy.setPosition( //mi posiziona la finestra nello schermo
      sf::Vector2i(0, static_cast<int>(window_height + 3 * padding))); //vettore a 2 dim fatto da interi
  window_xz.setPosition(sf::Vector2i(0, 0));
}

void initializeShapes(double wingspan, sf::CircleShape& boid_shape, //prende la forma del boid e del predatore e gli dice quanto è grande e il colore
                      sf::CircleShape& predator_shape) {
  float wingspanf = static_cast<float>(wingspan); //converto in un float

  boid_shape.setRadius(wingspanf);
  boid_shape.setFillColor(sf::Color::Black);

  predator_shape.setRadius(wingspanf * 2);
  predator_shape.setFillColor(sf::Color::Red);
}

void handleEvents(sf::RenderWindow& window) { //mi fa girare il programma finchè le finestre sono aperte
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

  double wingspan = swarm.wingspan();

  std::for_each(swarm.begin(), swarm.end(), [&](const Prey& prey) { //da inizio a fine stormo prendi delle prede e fagli questa cosa
    sf::Vector2<double> position;

    switch (plane) { //farlo contemporaneamente su due finestre ralleta l'esecuzione
    case 0:
      position.x = prey.position().x_ / screen.x_ * width - wingspan;
      position.y = prey.position().y_ / screen.y_ * height - wingspan;
      break;
    case 1:
      position.x = prey.position().x_ / screen.x_ * width - wingspan;
      position.y = prey.position().z_ / screen.z_ * height - wingspan;
      break;
    default: throw std::out_of_range("index out of range");
    }

    boid_shape.setPosition(sf::Vector2f(position));
    window.draw(boid_shape);
  });

  if (predator) {
    sf::Vector2<double> predator_position;
    switch (plane) {
    case 0:
      predator_position.x =
          predator->position().x_ / screen.x_ * width - wingspan * 2;
      predator_position.y =
          predator->position().y_ / screen.y_ * height - wingspan * 2;
      break;
    case 1:
      predator_position.x =
          predator->position().x_ / screen.x_ * width - wingspan * 2;
      predator_position.y =
          predator->position().z_ / screen.z_ * height - wingspan * 2;
      break;
    default: throw std::out_of_range("index out of range");
    }

    predator_shape.setPosition(sf::Vector2f(predator_position));
    window.draw(predator_shape);
  }
}

static void printStatistics(const Swarm& swarm, int t, //stampa su file
                            const std::string& filename) {
  std::vector<double> distances;
  std::vector<double> velocities;

  for (int i = 0; i < swarm.size(); ++i) { //non posso fare pray:swarm ma così mi garantisce che siano sempre diversi i boid per fare due for annidati
    velocities.push_back(swarm[i].velocity().norm()
                         * ConversionFactors::speed_k);
    for (int j = i + 1; j < swarm.size(); ++j) {
      distances.push_back(swarm[i].position().distance(
          swarm.toroidal(), swarm[j].position(), swarm.screen()) * ConversionFactors::space_k); //ci eravamo dimenticati uno space conversion
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
        << std::setw(6) //formatta i fle così la singola colonna ha una dimensione specifica
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
