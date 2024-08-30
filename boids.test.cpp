#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "vec3.hpp"

TEST_CASE("Testing Vec3 constructors")
{
  Vec3 v1;
  CHECK(v1.x == 0);
  CHECK(v1.y == 0);
  CHECK(v1.z == 0);

  Vec3 v2(1, 2, 3);
  CHECK(v2.x == 1);
  CHECK(v2.y == 2);
  CHECK(v2.z == 3);
}

TEST_CASE("Testing Vec3 equality and inequality operators")
{
  Vec3 v1(1, 2, 3);
  Vec3 v2(1, 2, 3);
  Vec3 v3(4, 5, 6);

  CHECK(v1 == v2);
  CHECK(v1 != v3);
}

TEST_CASE("Testing Vec3 addition and subtraction")
{
  Vec3 v1(1, 2, 3);
  Vec3 v2(4, 5, 6);

  Vec3 v3 = v1 + v2;
  CHECK(v3.x == 5);
  CHECK(v3.y == 7);
  CHECK(v3.z == 9);

  v1 += v2;
  CHECK(v1.x == 5);
  CHECK(v1.y == 7);
  CHECK(v1.z == 9);

  Vec3 v4 = v1 - v2;
  CHECK(v4.x == 1);
  CHECK(v4.y == 2);
  CHECK(v4.z == 3);

  v1 -= v2;
  CHECK(v1.x == 1);
  CHECK(v1.y == 2);
  CHECK(v1.z == 3);
}

TEST_CASE("Testing Vec3 scalar multiplication and division")
{
  Vec3 v1(1, 2, 3);

  Vec3 v2 = v1 * 2;
  CHECK(v2.x == 2);
  CHECK(v2.y == 4);
  CHECK(v2.z == 6);

  v1 *= 2;
  CHECK(v1.x == 2);
  CHECK(v1.y == 4);
  CHECK(v1.z == 6);

  Vec3 v3 = v1 / 2;
  CHECK(v3.x == 1);
  CHECK(v3.y == 2);
  CHECK(v3.z == 3);

  v1 /= 2;
  CHECK(v1.x == 1);
  CHECK(v1.y == 2);
  CHECK(v1.z == 3);
}

TEST_CASE("Testing Vec3 norm and normalization")
{
  Vec3 v1(3, 4, 0);
  CHECK(v1.norm() == 5);

  Vec3 v2 = v1.normalize();
  CHECK(v2.x == doctest::Approx(0.6));
  CHECK(v2.y == doctest::Approx(0.8));
  CHECK(v2.z == doctest::Approx(0.0));

  Vec3 v3(0, 0, 0);
  CHECK(v3.normalize() == v3);
}

TEST_CASE("Testing Vec3 dot and cross product")
{
  Vec3 v1(1, 0, 0);
  Vec3 v2(0, 1, 0);

  CHECK(v1.dot(v2) == 0);

  Vec3 v3 = v1.cross(v2);
  CHECK(v3.x == 0);
  CHECK(v3.y == 0);
  CHECK(v3.z == 1);

  Vec3 v4 = v2.cross(v1);
  CHECK(v4.x == 0);
  CHECK(v4.y == 0);
  CHECK(v4.z == -1);
}

TEST_CASE("Testing Vec3 element access")
{
  Vec3 v1(1, 2, 3);

  CHECK(v1[0] == 1);
  CHECK(v1[1] == 2);
  CHECK(v1[2] == 3);

  v1[0] = 4;
  v1[1] = 5;
  v1[2] = 6;

  CHECK(v1[0] == 4);
  CHECK(v1[1] == 5);
  CHECK(v1[2] == 6);

  CHECK_THROWS_AS(v1[3], std::out_of_range);
}

TEST_CASE("Testing Boid default constructor")
{
  Boid b;
  CHECK(b.get_position() == Vec3(0, 0, 0));
  CHECK(b.get_velocity() == Vec3(0, 0, 0));
}

TEST_CASE("Testing Boid parameterized constructor")
{
  Vec3 pos(1, 2, 3);
  Vec3 vel(4, 5, 6);
  Boid b(pos, vel);
  CHECK(b.get_position() == pos);
  CHECK(b.get_velocity() == vel);
}

TEST_CASE("Testing Boid copy constructor")
{
  Vec3 pos(1, 2, 3);
  Vec3 vel(4, 5, 6);
  Boid b1(pos, vel);
  Boid b2(b1);
  CHECK(b2.get_position() == pos);
  CHECK(b2.get_velocity() == vel);
}

TEST_CASE("Testing Boid assignment operator")
{
  Vec3 pos1(1, 2, 3);
  Vec3 vel1(4, 5, 6);
  Boid b1(pos1, vel1);

  Vec3 pos2(7, 8, 9);
  Vec3 vel2(10, 11, 12);
  Boid b2(pos2, vel2);

  b2 = b1;
  CHECK(b2.get_position() == pos1);
  CHECK(b2.get_velocity() == vel1);
}

TEST_CASE("Testing Boid position and velocity updates")
{
  Vec3 pos(1, 2, 3);
  Vec3 vel(1, 0, 0);
  Boid b(pos, vel);

  Vec3 new_pos(4, 5, 6);
  b.set_position(new_pos);
  CHECK(b.get_position() == new_pos);

  Vec3 new_vel(0, 1, 0);
  b.set_velocity(new_vel);
  CHECK(b.get_velocity() == new_vel);
}

TEST_CASE("Testing Boid velocity update with max speed")
{
  Vec3 pos(0, 0, 0);
  Vec3 vel(1, 2, 3);
  Boid b(pos, vel);

  Vec3 delta_v(2, 2, 2);
  double max_speed = 5.0;

  b.update_boid_velocity(delta_v, max_speed);
  CHECK(b.get_velocity().norm() <= max_speed);
}

TEST_CASE("Testing Boid position and velocity update with max speed")
{
  Vec3 pos(0, 0, 0);
  Vec3 vel(1, 2, 3);
  Boid b(pos, vel);

  Vec3 delta_v(2, 2, 2);
  double max_speed = 5.0;

  b.update_boid(delta_v, max_speed);
  CHECK(b.get_velocity().norm() <= max_speed);
  CHECK(b.get_position() == pos + b.get_velocity());
}

TEST_CASE("Testing Boid equality and inequality operators")
{
  Vec3 pos1(1, 2, 3);
  Vec3 vel1(4, 5, 6);
  Boid b1(pos1, vel1);

  Vec3 pos2(1, 2, 3);
  Vec3 vel2(4, 5, 6);
  Boid b2(pos2, vel2);

  Vec3 pos3(7, 8, 9);
  Vec3 vel3(10, 11, 12);
  Boid b3(pos3, vel3);

  CHECK(b1 == b2);
  CHECK(b1 != b3);
}

TEST_CASE("Testing toroidal_vec_dist and toroidal_distance")
{
  Vec3 width(10.0, 10.0, 10.0);

  Vec3 a(2.0, 3.0, 4.0);
  Vec3 b(7.0, 8.0, 9.0);
  Vec3 dist = toroidal_vec_dist(a, b, width);
  CHECK(dist == Vec3(-5.0, -5.0, -5.0));

  double toroidal_dist = toroidal_distance(a, b, width);
  CHECK(toroidal_dist == doctest::Approx(std::sqrt(75.0)));

  Vec3 c(9.0, 9.0, 0.0);
  Vec3 d(1.0, 1.0, 0.0);
  Vec3 wrapped_dist = toroidal_vec_dist(c, d, width);
  CHECK(wrapped_dist == Vec3(-2.0, -2.0, 0.0));

  double wrapped_toroidal_dist = toroidal_distance(c, d, width);
  CHECK(wrapped_toroidal_dist == doctest::Approx(std::sqrt(8.0)));
}

TEST_CASE("Testing distance function")
{
  Vec3 a(1.0, 2.0, 3.0);
  Vec3 b(4.0, 6.0, 8.0);

  double dist = distance(a, b);
  CHECK(dist == doctest::Approx(std::sqrt(50.0)));
}

TEST_CASE("Testing mean function")
{
  std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
  double calculated_mean     = mean(values);
  CHECK(calculated_mean == doctest::Approx(3.0));

  std::vector<double> empty_values;
  CHECK_THROWS_AS(mean(empty_values), std::runtime_error);
}

TEST_CASE("Testing dev_std function")
{
  std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
  double calculated_mean     = mean(values);
  double calculated_dev_std  = dev_std(values, calculated_mean);
  CHECK(calculated_dev_std == doctest::Approx(std::sqrt(2.0)));
}

TEST_CASE("Swarm initialization")
{
  Swarm swarm;
  CHECK(swarm.get_size() == 100);
  CHECK(swarm.get_wingspan() == 2);
  CHECK(swarm.get_max_speed() == 1);
  CHECK(swarm.get_min_distance() == 30);
  CHECK(swarm.get_cooldown() == 0);

  Boid predator(Vec3(10, 10, 10), Vec3(1, 1, 1));
  Swarm custom_swarm(50, 3, 2, 40, 100, 60, 70, 80, 90, predator,
                     Vec3(800, 600, 400), true, Vec3(1, 0, 0));
  CHECK(custom_swarm.get_size() == 50);
  CHECK(custom_swarm.get_wingspan() == 3);
  CHECK(custom_swarm.get_max_speed() == 2);
  CHECK(custom_swarm.get_min_distance() == 40);
  CHECK(custom_swarm.get_cohesion_factor() == 70);
  CHECK(custom_swarm.is_toroidal() == true);
  CHECK(custom_swarm.get_wind() == Vec3(1, 0, 0));
}

TEST_CASE("Boid interactions in the swarm")
{
  Swarm swarm(10, 2, 1, 30, 150, 50, 50, 50, 50);
  Boid yautja(Vec3(5, 5, 5), Vec3(0, 0, 0));

  CHECK(swarm.get_size() == 10);

  swarm.update_swarm(yautja);

  CHECK(swarm.get_size() <= 10);
}

TEST_CASE("Swarm boundary behavior")
{
  Boid predator(Vec3(50, 50, 50), Vec3(0, 0, 0));
  Swarm swarm(10, 2, 1, 30, 150, 50, 50, 50, 50, predator, Vec3(100, 100, 100),
              false);

  Boid& b = const_cast<Boid&>(swarm[0]);
  b.set_position(Vec3(-1, 50, 50));
  swarm.border(b);
  CHECK(b.get_position().x == 0);

  b.set_position(Vec3(101, 50, 50));
  swarm.border(b);
  CHECK(b.get_position().x == 100);

  Swarm toroidal_swarm(10, 2, 1, 30, 150, 50, 50, 50, 50, predator,
                       Vec3(100, 100, 100), true);
  Boid& tb = const_cast<Boid&>(toroidal_swarm[0]);
  tb.set_position(Vec3(101, 50, 50));
  toroidal_swarm.border(tb);
  CHECK(tb.get_position().x == 1);
}

TEST_CASE("Predator constructor and initialization")
{
  Predator default_predator;
  CHECK(default_predator.get_position() == Vec3(0, 0, 0));
  CHECK(default_predator.get_velocity().norm()
        <= default_predator.get_attack_speed());

  Predator param_predator(50, 70, Vec3(500, 500, 500), true, Vec3(0, 0, 0));
  CHECK(param_predator.get_velocity().norm()
        <= param_predator.get_attack_speed());
  CHECK(param_predator.get_attack_range() == 50);
  CHECK(param_predator.get_attack_speed() == 70);
  CHECK(param_predator.get_screen() == Vec3(500, 500, 500));
}

TEST_CASE("Predator update behavior")
{
  Predator predator(50, 1.3, Vec3(500, 500, 500), false, Vec3(0, 0, 0));

  Swarm swarm(2, 2.0, 1.0, 30.0, 150.0, 50.0, 50.0, 50.0, 50.0);

  swarm[0].set_position(Vec3(150, 100, 100));

  swarm[1].set_position(Vec3(500, 500, 500));

  predator.set_position(Vec3(100, 100, 100));

  predator.set_velocity(Vec3(10, 0, 0));

  CHECK(swarm.get_cooldown() == 0);

  predator.update_predator(swarm);

  CHECK(predator.get_velocity().normalize() == Vec3(1, 0, 0));
}

TEST_CASE("Testing casual_parameters function")
{
  double wingspan, max_speed, min_distance, separation_factor, cohesion_factor;
  double alignment_factor, fear_factor, sight_distance, attack_speed,
      attack_range;

  casual_parameters(wingspan, max_speed, min_distance, separation_factor,
                    cohesion_factor, alignment_factor, fear_factor,
                    sight_distance, attack_speed, attack_range);

  CHECK(wingspan >= 0.2);
  CHECK(wingspan <= 5.0);
  CHECK(max_speed >= 0.001);
  CHECK(max_speed <= 2.0);
  CHECK(min_distance >= wingspan);
  CHECK(min_distance <= 100.0);
  CHECK(separation_factor >= 0.000001);
  CHECK(separation_factor <= 0.1);
  CHECK(cohesion_factor >= 0.000000001);
  CHECK(cohesion_factor <= 0.0001);
  CHECK(alignment_factor >= 0.0000001);
  CHECK(alignment_factor <= 0.01);
  CHECK(fear_factor >= 0.000001);
  CHECK(fear_factor <= 0.1);
  CHECK(sight_distance >= wingspan);
  CHECK(sight_distance <= 500.0);
  CHECK(attack_speed >= 0.001);
  CHECK(attack_speed <= 2.0);
  CHECK(attack_range >= 0.001);
  CHECK(attack_range <= 500.0);
}

TEST_CASE("Testing initialize_parameters function")
{
  bool manually{};
  std::vector<Boid>::size_type size{};
  double wingspan{}, max_speed{}, min_distance{}, separation_factor{},
      cohesion_factor{};
  double alignment_factor{}, fear_factor{}, sight_distance{}, attack_speed{},
      attack_range{};
  bool toroidalbool{};
  Vec3 wind(0, 0, 0);

  std::istringstream input("1\ny\nN\nn\n");
  std::cin.rdbuf(input.rdbuf());

  initialize_parameters(manually, size, wingspan, max_speed, min_distance,
                        separation_factor, cohesion_factor, alignment_factor,
                        fear_factor, sight_distance, toroidalbool, wind,
                        attack_speed, attack_range);

  CHECK(!manually);
  CHECK(size == 0);              // Default value
  CHECK(wingspan == 0);          // Default value
  CHECK(max_speed == 0);         // Default value
  CHECK(min_distance == 0);      // Default value
  CHECK(separation_factor == 0); // Default value
  CHECK(cohesion_factor == 0);   // Default value
  CHECK(alignment_factor == 0);  // Default value
  CHECK(fear_factor == 0);       // Default value
  CHECK(sight_distance == 0);    // Default value
  CHECK(toroidalbool == false);  // Default value
  CHECK(wind.norm() == 0.02);    // Default value
  CHECK(attack_speed == 0);      // Default value
  CHECK(attack_range == 0);      // Default value

  input.str("n\n");
  input.clear();
  std::cin.rdbuf(input.rdbuf());

  initialize_parameters(manually, size, wingspan, max_speed, min_distance,
                        separation_factor, cohesion_factor, alignment_factor,
                        fear_factor, sight_distance, toroidalbool, wind,
                        attack_speed, attack_range);

  CHECK(!manually);
}

TEST_CASE("Testing draw_windows function")
{
  sf::RenderWindow windowXY, windowXZ;
  draw_windows(windowXY, windowXZ);

  CHECK(windowXY.isOpen());
  CHECK(windowXZ.isOpen());

  sf::Vector2u sizeXY = windowXY.getSize();
  sf::Vector2u sizeXZ = windowXZ.getSize();

  CHECK(sizeXY.x > 0);
  CHECK(sizeXY.y > 0);
  CHECK(sizeXZ.x > 0);
  CHECK(sizeXZ.y > 0);
}

TEST_CASE("Testing initialize_shapes function")
{
  float wingspan = 5.0f;
  sf::CircleShape boid_shape, predator_shape;
  initialize_shapes(wingspan, boid_shape, predator_shape);

  CHECK(boid_shape.getRadius() == wingspan);
  CHECK(boid_shape.getFillColor() == sf::Color::White);

  CHECK(predator_shape.getRadius() == wingspan * 2);
  CHECK(predator_shape.getFillColor() == sf::Color::Red);
}

TEST_CASE("Testing handle_events function")
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "Test Window");

  sf::Event event;
  window.pollEvent(event);
  window.close();

  handle_events(window);

  CHECK(!window.isOpen());
}

TEST_CASE("Testing print_statistics function")
{
  Swarm swarm;
  int t = 100;

  std::ostringstream oss;
  std::streambuf* orig_buf = std::cout.rdbuf();
  std::cout.rdbuf(oss.rdbuf());

  print_statistics(swarm, t);

  std::cout.rdbuf(orig_buf);
  std::string output = oss.str();

  CHECK(output.find("t = 100") != std::string::npos);
}