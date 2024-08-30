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
#include <string>
#include <vector>

// Funzione per inizializzare i parametri basandosi sull'input dell'utente
void initialize_parameters(bool& manually, std::vector<boid>::size_type& size,
                           int& wingspan, double& max_speed,
                           double& min_distance, double& separation_factor,
                           double& cohesion_factor, double& alignment_factor,
                           double& fear_factor, double& sight_distance,
                           bool& toroidalbool, vec3& wind, double& attack_speed,
                           double& attack_range)
{
  std::string input;
  bool windbool;
  double windspeed{0.02};

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
    std::cout << "Enter swarm size: ";
    std::vector<boid>::size_type size_;
    std::cin >> size_;
    if (size_ <= 1 || !(std::cin >> size_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      size = size_;
    }
    std::cout << "Enter wingspan(0 ~ 5): ";
    int wingspan_;
    std::cin >> wingspan_;
    if (wingspan_ <= 0 || wingspan_ > 5 || !(std::cin >> wingspan_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      wingspan = wingspan_;
    }
    std::cout << "Enter maximum speed(0 ~ 200): ";
    double max_speed_;
    std::cin >> max_speed_;
    if (max_speed_ <= 0 || max_speed_ > 200 || !(std::cin >> max_speed_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      max_speed = max_speed_ / 100;
    }
    std::cout << "Enter minimum distance(0 ~ 100): ";
    double min_distance_;
    std::cin >> min_distance_;
    if (min_distance_ <= 0 || min_distance_ > 100
        || !(std::cin >> min_distance_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      min_distance = min_distance_;
    }
    std::cout << "Enter separation factor (0 ~ 100): ";
    double separation_factor_;
    std::cin >> separation_factor_;
    if (separation_factor_ <= 0 || separation_factor_ > 100
        || !(std::cin >> separation_factor_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      separation_factor = separation_factor_ / 1000;
    }
    std::cout << "Enter cohesion factor (0 ~ 100): ";
    double cohesion_factor_;
    std::cin >> cohesion_factor_;
    if (cohesion_factor_ <= 0 || cohesion_factor_ > 100
        || !(std::cin >> cohesion_factor_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      cohesion_factor = cohesion_factor_ / 1000000;
    }
    std::cout << "Enter alignment factor (0 ~ 100): ";
    double alignment_factor_;
    std::cin >> alignment_factor_;
    if (alignment_factor_ <= 0 || alignment_factor_ > 100
        || !(std::cin >> alignment_factor_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      alignment_factor = alignment_factor_ / 10000;
    }
    std::cout << "Enter fear factor (0 ~ 100): ";
    double fear_factor_;
    std::cin >> fear_factor_;
    if (fear_factor_ <= 0 || fear_factor_ > 100
        || !(std::cin >> fear_factor_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      fear_factor = fear_factor_ / 1000;
    }
    std::cout << "Enter sight distance(0 ~ 500): ";
    double sight_distance_;
    std::cin >> sight_distance_;
    if (sight_distance_ <= 0 || sight_distance_ > 500
        || !(std::cin >> sight_distance_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      sight_distance = sight_distance_;
    }
    std::cout << "Enter predator attack speed (0 ~ 200): ";
    double attack_speed_;
    std::cin >> attack_speed_;
    if (attack_speed_ <= 0 || attack_speed_ > 200
        || !(std::cin >> attack_speed_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      attack_speed = attack_speed_ / 100;
    }
    std::cout << "Enter predator attack range(0 ~ 500): ";
    double attack_range_;
    std::cin >> attack_range_;
    if (attack_range_ <= 0 || attack_range_ > 500
        || !(std::cin >> attack_range_)) {
      std::cout << "This value is not accetable, setted to default value\n";
    } else {
      attack_range = attack_range_;
    }
    if (windbool) {
      std::cout << "Enter wind speed(0 ~ 20): ";
      double windspeed_;
      std::cin >> windspeed_;
      if (windspeed_ <= 0 || windspeed_ > 20 || !(std::cin >> windspeed_)) {
        std::cout << "This value is not accetable, setted to default value\n";
      } else {
        windspeed = windspeed_ / 100;
      }
    }
  }

  if (windbool) {
    std::srand(static_cast<unsigned>(std::time(0)));
    wind = vec3((std::rand() % 100) - 50, (std::rand() % 100) - 50,
                (std::rand() % 20) - 10)
               .normalize()
         * windspeed;
  }
}

// disegna le finestre
void draw_windows(sf::RenderWindow& windowXY_, sf::RenderWindow& windowXZ_)
{
  // Get the desktop resolution
  const sf::VideoMode& desktopMode = sf::VideoMode::getDesktopMode();
  unsigned int screenWidth         = desktopMode.width;
  unsigned int screenHeight        = desktopMode.height;

  // Define padding and application bar height (adjust as needed)
  unsigned int padding = 12;
  unsigned int appBarHeight =
      35; // Assume the application bar is 35 pixels high

  // Calculate the size for each window, slightly smaller than the quarter
  // screen
  unsigned int windowWidth  = (screenWidth / 2) - padding;
  unsigned int windowHeight = (screenHeight / 2) - 2 * padding - appBarHeight;

  // Create the windows with the calculated sizes
  windowXY_.create(sf::VideoMode(windowWidth, windowHeight), "XY Plane");
  windowXZ_.create(sf::VideoMode(windowWidth, windowHeight), "XZ Plane");

  // Set the position of the windows, with offset to account for padding and app
  // bar
  windowXY_.setPosition(sf::Vector2i(
      0, static_cast<int>(windowHeight + 3 * padding))); // Bottom-left corner
  windowXZ_.setPosition(sf::Vector2i(0, 0));             // Upper-left corner
}

void initialize_shapes(float boidWingspan, sf::CircleShape& boidShape,
                       sf::CircleShape& predatorShape)
{
  boidShape.setRadius(boidWingspan);
  boidShape.setFillColor(sf::Color::White);

  predatorShape.setRadius(boidWingspan * 2);
  predatorShape.setFillColor(sf::Color::Red);
}

// disegna i boids nella finestra
void draw_boids_on_plane(const swarm& boids, const predator& yautja,
                         sf::RenderWindow& window, const int plane,
                         sf::CircleShape& boidShape,
                         sf::CircleShape& predatorShape)
{
  if (plane > 1) {
    throw std::out_of_range("index out of range");
  }

  const vec3& screen = boids.get_screen();
  double width       = window.getSize().x;
  double height      = window.getSize().y;

  // Disegno dei boids
  for (std::vector<boid>::size_type i = 0; i < boids.get_size(); i++) {
    sf::Vector2<double> position;

    switch (plane) {
    case 0:
      position.x = boids[i].get_position().x / screen.x * width;
      position.y = boids[i].get_position().y / screen.y * height;
      break;
    case 1:
      position.x = boids[i].get_position().x / screen.x * width;
      position.y = boids[i].get_position().z / screen.z * height;
      break;
    }

    boidShape.setPosition(sf::Vector2f(position));
    window.draw(boidShape);
  }

  // Disegno del predator
  sf::Vector2<double> predatorPosition;
  switch (plane) {
  case 0:
    predatorPosition.x = yautja.get_position().x / screen.x * width;
    predatorPosition.y = yautja.get_position().y / screen.y * height;
    break;
  case 1:
    predatorPosition.x = yautja.get_position().x / screen.x * width;
    predatorPosition.y = yautja.get_position().z / screen.z * height;
    break;
  }

  predatorShape.setPosition(sf::Vector2f(predatorPosition));
  window.draw(predatorShape);
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
void print_statistics(swarm& boids_, int t_)
{
  std::vector<double> distances, velocities;
  for (std::vector<boid>::size_type i = 0; i < boids_.get_size(); ++i) {
    velocities.push_back(boids_[i].get_velocity().norm()*100);
    for (std::vector<boid>::size_type j = i + 1; j < boids_.get_size(); ++j) {
      distances.push_back(
          boids_.is_toroidal()
              ? toroidal_distance(boids_[i].get_position(),
                                  boids_[j].get_position(), boids_.get_screen())
              : (boids_[i].get_position() - boids_[j].get_position()).norm());
    }
  }
  double mean_dist = mean(distances), mean_vel = mean(velocities);
  std::cout << "t = " << t_ << "; <d> = " << mean_dist
            << "; sd = " << dev_std(distances, mean_dist)
            << "; <v> = " << mean_vel
            << "; sv  = " << dev_std(velocities, mean_vel)
            << "; n  = " << boids_.get_size() << std::endl;
}

// update del predator e dei boids e stampa delle statistiche a intervalli
// regolari
void update_simulation(predator& yautja_, swarm& boids_, int& t_)
{
  yautja_.update_predator(boids_);
  boids_.update_swarm(yautja_);

  if (t_ % 100 == 0) {
    print_statistics(boids_, t_);
  }
  ++t_;
}

#endif
