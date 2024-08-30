#ifndef FUNCTIONS
#define FUNCTIONS

#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "vec3.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

// Funzione per la generazione casuale dei parametri
void casual_parameters(double& wingspan, double& max_speed,
                       double& min_distance, double& separation_factor,
                       double& cohesion_factor, double& alignment_factor,
                       double& fear_factor, double& sight_distance,
                       double& attack_speed, double& attack_range, double& windspeed)
{
  // Set casual values for the parameters
  std::random_device rd;  // Obtain a random number from hardware
  std::mt19937 gen(rd()); // Seed the generator
  std::cout << "Casual parameters generation:\n";
  std::uniform_real_distribution<> dis(0.2, 5); // Define the range
  wingspan = dis(gen);

  std::uniform_real_distribution<> dis1(0.001, 2); // Define the range
  max_speed = dis1(gen);

  std::uniform_real_distribution<> dis2(wingspan, 100); // Define the range
  min_distance = dis2(gen);

  std::uniform_real_distribution<> dis3(0.001, 100); // Define the range
  separation_factor = dis3(gen) / 1000;

  std::uniform_real_distribution<> dis4(0.001, 100); // Define the range
  cohesion_factor = dis4(gen) / 1000000;

  std::uniform_real_distribution<> dis5(0.001, 100); // Define the range
  alignment_factor = dis5(gen) / 10000;

  std::uniform_real_distribution<> dis6(0.001, 100); // Define the range
  fear_factor = dis6(gen) / 1000;

  std::uniform_real_distribution<> dis7(wingspan, 500); // Define the range
  sight_distance = dis7(gen);

  std::uniform_real_distribution<> dis8(0.001, 200); // Define the range
  attack_speed = dis8(gen) / 100;

  std::uniform_real_distribution<> dis9(0.001, 500); // Define the range
  attack_range = dis9(gen);

  std::uniform_real_distribution<> dis10(0.001, 20); // Define the range
  windspeed = dis10(gen) / 100;
}

// Funzione per inizializzare i parametri basandosi sull'input dell'utente
void initialize_parameters(bool& manually, std::vector<Boid>::size_type& size,
                           double& wingspan, double& max_speed,
                           double& min_distance, double& separation_factor,
                           double& cohesion_factor, double& alignment_factor,
                           double& fear_factor, double& sight_distance,
                           bool& toroidalbool, Vec3& wind, double& attack_speed,
                           double& attack_range)
{
  std::string input;
  bool windbool;
  double windspeed{0.02};

  std::cout << "Enter swarm size: ";
  std::vector<Boid>::size_type size_;
  std::cin >> size_;
  if (size_ <= 1) {
    std::cout << "This value is not accetable, setted to default value\n";
  } else {
    size = size_;
  }

  std::cout << "Enable wind? [y/N]: ";
  std::cin >> input;
  windbool = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Enable toroidal space? (Recommended with wind enabled) [y/N]: ";
  std::cin >> input;
  toroidalbool =
      (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  std::cout << "Insert parameters manually? [y/N]: ";
  std::cin >> input;
  manually = (input == "y" || input == "Y" || input == "yes" || input == "Yes");

  if (manually) {
    std::cout << "Enter wingspan(0 ~ 5): ";
    double wingspan_;
    std::cin >> wingspan_;
    if (wingspan_ <= 0 || wingspan_ > 5) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      wingspan = wingspan_;
    }
    std::cout << "Enter maximum speed(0 ~ 200): ";
    double max_speed_;
    std::cin >> max_speed_;
    if (max_speed_ <= 0 || max_speed_ > 200) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      max_speed = max_speed_ / 100;
    }
    std::cout << "Enter minimum distance(0 ~ 100): ";
    double min_distance_;
    std::cin >> min_distance_;
    if (min_distance_ <= 0 || min_distance_ > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      min_distance = min_distance_;
    }
    std::cout << "Enter separation factor (0 ~ 100): ";
    double separation_factor_;
    std::cin >> separation_factor_;
    if (separation_factor_ <= 0 || separation_factor_ > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      separation_factor = separation_factor_ / 1000;
    }
    std::cout << "Enter cohesion factor (0 ~ 100): ";
    double cohesion_factor_;
    std::cin >> cohesion_factor_;
    if (cohesion_factor_ <= 0 || cohesion_factor_ > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      cohesion_factor = cohesion_factor_ / 1000000;
    }
    std::cout << "Enter alignment factor (0 ~ 100): ";
    double alignment_factor_;
    std::cin >> alignment_factor_;
    if (alignment_factor_ <= 0 || alignment_factor_ > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      alignment_factor = alignment_factor_ / 10000;
    }
    std::cout << "Enter fear factor (0 ~ 100): ";
    double fear_factor_;
    std::cin >> fear_factor_;
    if (fear_factor_ <= 0 || fear_factor_ > 100) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      fear_factor = fear_factor_ / 1000;
    }
    std::cout << "Enter sight distance(0 ~ 500): ";
    double sight_distance_;
    std::cin >> sight_distance_;
    if (sight_distance_ <= 0 || sight_distance_ > 500) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      sight_distance = sight_distance_;
    }
    std::cout << "Enter predator attack speed (0 ~ 200): ";
    double attack_speed_;
    std::cin >> attack_speed_;
    if (attack_speed_ <= 0 || attack_speed_ > 200) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      attack_speed = attack_speed_ / 100;
    }
    std::cout << "Enter predator attack range(0 ~ 500): ";
    double attack_range_;
    std::cin >> attack_range_;
    if (attack_range_ <= 0 || attack_range_ > 500) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      attack_range = attack_range_;
    }
    if (windbool) {
      std::cout << "Enter wind speed(0 ~ 20): ";
      double windspeed_;
      std::cin >> windspeed_;
      if (windspeed_ <= 0 || windspeed_ > 20) {
        std::cout << "This value is not accetable, setted to default value\n";
      } else {
        windspeed = windspeed_ / 100;
      }
    }
  } else { // casual parameters
    bool casual;
    std::cout << "casual parameters geneation? [y/N]: ";
    std::cin >> input;
    casual = (input == "y" || input == "Y" || input == "yes" || input == "Yes");
    if (casual) {
      casual_parameters(wingspan, max_speed, min_distance, separation_factor,
                        cohesion_factor, alignment_factor, fear_factor,
                        sight_distance, attack_speed, attack_range, windspeed);
    }
    std::cout << "Parameters setted at values:\n";
    std::cout << "Wingspan: " << wingspan << std::endl;
    std::cout << "Max speed: " << max_speed * 100 << std::endl;
    std::cout << "Min distance: " << min_distance << std::endl;
    std::cout << "Separation factor: " << separation_factor * 1000 << std::endl;
    std::cout << "Cohesion factor: " << cohesion_factor * 1000000 << std::endl;
    std::cout << "Alignment factor: " << alignment_factor * 10000 << std::endl;
    std::cout << "Fear factor: " << fear_factor * 1000 << std::endl;
    std::cout << "Sight distance: " << sight_distance << std::endl;
    std::cout << "Attack speed: " << attack_speed * 100 << std::endl;
    std::cout << "Attack range: " << attack_range << std::endl;
  }
  if (windbool) {
  std::random_device rd;  // Obtain a random number from hardware
  std::mt19937 gen(rd()); // Seed the generator
  std::uniform_real_distribution<> dis_x(-50, 50); // Define the range
  std::uniform_real_distribution<> dis_y(-25, 25);
  std::uniform_real_distribution<> dis_z(-10, 10);
  wind = Vec3(dis_x(gen), dis_y(gen), dis_z(gen)).normalize() * windspeed;
  std::cout << "Wind Speed: " << windspeed << std::endl;
  std::cout << "Wind: " << (wind*100).to_string() << std::endl;
  }
  
}

// disegna le finestre
void draw_windows(sf::RenderWindow& windowXY_, sf::RenderWindow& windowXZ_)
{
  // Get the desktop resolution
  const sf::VideoMode& desktop_mode = sf::VideoMode::getDesktopMode();
  unsigned int screen_width         = desktop_mode.width;
  unsigned int screen_height        = desktop_mode.height;

  // Define padding and application bar height (adjust as needed)
  unsigned int padding = 12;
  unsigned int app_bar_height =
      35; // Assume the application bar is 35 pixels high

  // Calculate the size for each window, slightly smaller than the quarter
  // screen
  unsigned int window_width = (screen_width / 2) - padding;
  unsigned int window_height =
      (screen_height / 2) - 2 * padding - app_bar_height;

  // Create the windows with the calculated sizes
  windowXY_.create(sf::VideoMode(window_width, window_height), "XY Plane");
  windowXZ_.create(sf::VideoMode(window_width, window_height), "XZ Plane");

  // Set the position of the windows, with offset to account for padding and app
  // bar
  windowXY_.setPosition(sf::Vector2i(
      0, static_cast<int>(window_height + 3 * padding))); // Bottom-left corner
  windowXZ_.setPosition(sf::Vector2i(0, 0));              // Upper-left corner
}

void initialize_shapes(float wingspan_, sf::CircleShape& boid_shape_,
                       sf::CircleShape& predator_shape_)
{
  boid_shape_.setRadius(wingspan_);
  boid_shape_.setFillColor(sf::Color::White);

  predator_shape_.setRadius(wingspan_ * 2);
  predator_shape_.setFillColor(sf::Color::Red);
}

// disegna i boids nella finestra
void draw_boids_on_plane(const Swarm& boids_, const Predator& yautja_,
                         sf::RenderWindow& window, const int plane,
                         sf::CircleShape& boid_shape_,
                         sf::CircleShape& predator_shape_)
{
  if (plane > 1) {
    throw std::out_of_range("index out of range");
  }

  const Vec3& screen = boids_.get_screen();
  double width       = window.getSize().x;
  double height      = window.getSize().y;

  // Disegno dei boids
  for (std::vector<Boid>::size_type i = 0; i < boids_.get_size(); ++i) {
    sf::Vector2<double> position;

    switch (plane) {
    case 0:
      position.x = boids_[i].get_position().x / screen.x * width;
      position.y = boids_[i].get_position().y / screen.y * height;
      break;
    case 1:
      position.x = boids_[i].get_position().x / screen.x * width;
      position.y = boids_[i].get_position().z / screen.z * height;
      break;
    }

    boid_shape_.setPosition(sf::Vector2f(position));
    window.draw(boid_shape_);
  }

  // Disegno del predator
  sf::Vector2<double> predator_position;
  switch (plane) {
  case 0:
    predator_position.x = yautja_.get_position().x / screen.x * width;
    predator_position.y = yautja_.get_position().y / screen.y * height;
    break;
  case 1:
    predator_position.x = yautja_.get_position().x / screen.x * width;
    predator_position.y = yautja_.get_position().z / screen.z * height;
    break;
  }

  predator_shape_.setPosition(sf::Vector2f(predator_position));
  window.draw(predator_shape_);
}

// interruzione del programma alla chiusura di una finestra
void handle_events(sf::RenderWindow& window)
{
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
  }
}

// stampa a schermo delle statistiche
void print_statistics(Swarm& boids_, int t_)
{
  std::vector<double> distances, velocities;
  for (std::vector<Boid>::size_type i = 0; i < boids_.get_size(); ++i) {
    velocities.push_back(boids_[i].get_velocity().norm() * 100);
    for (std::vector<Boid>::size_type j = i + 1; j < boids_.get_size(); ++j) {
      distances.push_back(
          boids_.is_toroidal()
              ? toroidal_distance(boids_[i].get_position(),
                                  boids_[j].get_position(), boids_.get_screen())
              : (boids_[i].get_position() - boids_[j].get_position()).norm());
    }
  }
  double mean_dist = mean(distances), mean_vel = mean(velocities);
  std::cout << "t = " << t_ << "; mean_distance = " << mean_dist
            << "; distance_std_dev = " << dev_std(distances, mean_dist)
            << "; mean_velocity = " << mean_vel
            << "; velocity_std_dev  = " << dev_std(velocities, mean_vel)
            << "; n_boids  = " << boids_.get_size() << std::endl;
}

// update del predator e dei boids e stampa delle statistiche a intervalli
// regolari
void update_simulation(Predator& yautja_, Swarm& boids_, int& t_)
{
  yautja_.update_predator(boids_);
  boids_.update_swarm(yautja_);

  if (t_ % 100 == 0) {
    print_statistics(boids_, t_);
  }
  ++t_;
}

#endif