#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "boid.hpp"
#include "functions.hpp"
#include "predator.hpp"
#include "statistics.hpp"
#include "swarm.hpp"
#include "variables.hpp"
#include "vec3.hpp"

TEST_CASE("Vec3 Default Constructor") {
    boids::Vec3 v;
    CHECK(v.x() == 0.0);
    CHECK(v.y() == 0.0);
    CHECK(v.z() == 0.0);
}

TEST_CASE("Vec3 Parameterized Constructor") {
    boids::Vec3 v(1.0, 2.0, 3.0);
    CHECK(v.x() == 1.0);
    CHECK(v.y() == 2.0);
    CHECK(v.z() == 3.0);
}

TEST_CASE("Vec3 Copy Constructor") {
    boids::Vec3 v1(1.0, 2.0, 3.0);
    boids::Vec3 v2(v1);
    CHECK(v2 == v1);
}

TEST_CASE("Vec3 Assignment Operator") {
    boids::Vec3 v1(1.0, 2.0, 3.0);
    boids::Vec3 v2;
    v2 = v1;
    CHECK(v2 == v1);
}

TEST_CASE("Vec3 Equality Operator") {
    boids::Vec3 v1(1.0, 2.0, 3.0);
    boids::Vec3 v2(1.0, 2.0, 3.0);
    CHECK(v1 == v2);
}

TEST_CASE("Vec3 Addition Operator") {
    boids::Vec3 v1(1.0, 2.0, 3.0);
    boids::Vec3 v2(4.0, 5.0, 6.0);
    boids::Vec3 v3 = v1 + v2;
    CHECK(v3 == boids::Vec3(5.0, 7.0, 9.0));
}

TEST_CASE("Vec3 Subtraction Operator") {
    boids::Vec3 v1(4.0, 5.0, 6.0);
    boids::Vec3 v2(1.0, 2.0, 3.0);
    boids::Vec3 v3 = v1 - v2;
    CHECK(v3 == boids::Vec3(3.0, 3.0, 3.0));
}

TEST_CASE("Vec3 Multiplication by Scalar") {
    boids::Vec3 v(1.0, 2.0, 3.0);
    boids::Vec3 v2 = v * 2.0;
    CHECK(v2 == boids::Vec3(2.0, 4.0, 6.0));
}

TEST_CASE("Vec3 Division by Scalar") {
    boids::Vec3 v(2.0, 4.0, 6.0);
    boids::Vec3 v2 = v / 2.0;
    CHECK(v2 == boids::Vec3(1.0, 2.0, 3.0));
}

TEST_CASE("Vec3 Out of Range Indexing") {
    boids::Vec3 v(1.0, 2.0, 3.0);
    CHECK_THROWS_AS(v[3], std::out_of_range);
}

TEST_CASE("Vec3 Norm Calculation") {
    boids::Vec3 v(3.0, 4.0, 0.0);
    CHECK(v.norm() == doctest::Approx(5.0));

    boids::Vec3 v2(1.0, 2.0, 2.0);
    CHECK(v2.norm() == doctest::Approx(3.0));
}

TEST_CASE("Vec3 Normalize") {
    boids::Vec3 v(3.0, 4.0, 0.0);
    boids::Vec3 norm_v = v.normalize();
    CHECK(norm_v.x() == doctest::Approx(0.6));
    CHECK(norm_v.y() == doctest::Approx(0.8));
    CHECK(norm_v.z() == doctest::Approx(0.0));
    CHECK(norm_v.norm() == doctest::Approx(1.0));

    boids::Vec3 zero_v(0.0, 0.0, 0.0);
    boids::Vec3 norm_zero_v = zero_v.normalize();
    CHECK(norm_zero_v == zero_v);
}

TEST_CASE("Vec3 NormalizeAssign") {
    boids::Vec3 v(3.0, 4.0, 0.0);
    v.normalizeAssign();
    CHECK(v.x() == doctest::Approx(0.6));
    CHECK(v.y() == doctest::Approx(0.8));
    CHECK(v.z() == doctest::Approx(0.0));
    CHECK(v.norm() == doctest::Approx(1.0));
}

TEST_CASE("Vec3 Dot Product") {
    boids::Vec3 v1(1.0, 0.0, 0.0);
    boids::Vec3 v2(0.0, 1.0, 0.0);
    CHECK(v1.dot(v2) == doctest::Approx(0.0));

    boids::Vec3 v3(1.0, 2.0, 3.0);
    boids::Vec3 v4(4.0, -5.0, 6.0);
    CHECK(v3.dot(v4) == doctest::Approx(12.0));
}

TEST_CASE("Vec3 Cross Product") {
    boids::Vec3 v1(1.0, 0.0, 0.0);
    boids::Vec3 v2(0.0, 1.0, 0.0);
    boids::Vec3 cross_v = v1.cross(v2);
    CHECK(cross_v == boids::Vec3(0.0, 0.0, 1.0));

    boids::Vec3 v3(3.0, -3.0, 1.0);
    boids::Vec3 v4(4.0, 9.0, 2.0);
    boids::Vec3 cross_v2 = v3.cross(v4);
    CHECK(cross_v2 == boids::Vec3(-15.0, -2.0, 39.0));
}

TEST_CASE("Boid Default Constructor") {
    boids::Boid b;
    CHECK(b.position() == boids::Vec3(0.0, 0.0, 0.0));
    CHECK(b.velocity() == boids::Vec3(0.0, 0.0, 0.0));
}

TEST_CASE("Boid Parameterized Constructor") {
    boids::Vec3 pos(1.0, 2.0, 3.0);
    boids::Vec3 vel(0.5, 0.5, 0.5);
    boids::Boid b(pos, vel);
    
    CHECK(b.position() == pos);
    CHECK(b.velocity() == vel);
}

TEST_CASE("Boid Copy Constructor") {
    boids::Vec3 pos(1.0, 2.0, 3.0);
    boids::Vec3 vel(0.5, 0.5, 0.5);
    boids::Boid b1(pos, vel);
    boids::Boid b2(b1);
    
    CHECK(b2 == b1);
}

TEST_CASE("Boid Assignment Operator") {
    boids::Vec3 pos(1.0, 2.0, 3.0);
    boids::Vec3 vel(0.5, 0.5, 0.5);
    boids::Boid b1(pos, vel);
    boids::Boid b2;
    
    b2 = b1;
    CHECK(b2 == b1);
}

TEST_CASE("Boid Position and Velocity Setters") {
    boids::Boid b;
    boids::Vec3 new_pos(1.0, 1.0, 1.0);
    boids::Vec3 new_vel(0.1, 0.2, 0.3);
    
    b.set_position(new_pos);
    b.set_velocity(new_vel);
    
    CHECK(b.position() == new_pos);
    CHECK(b.velocity() == new_vel);
}

TEST_CASE("Boid Velocity Update with max_speed") {
    boids::Vec3 pos(0.0, 0.0, 0.0);
    boids::Vec3 vel(1.0, 1.0, 1.0);
    boids::Boid b(pos, vel);

    boids::Vec3 delta_v(1.0, 1.0, 1.0);
    double max_speed = 2.0;
    
    b.updateBoidVelocity(delta_v, max_speed);

    // Ensure the velocity norm is clamped to max_speed
    CHECK(b.velocity().norm() == doctest::Approx(max_speed));
    CHECK(b.velocity() == boids::Vec3(1.1547, 1.1547, 1.1547)); // Normalized and scaled version of new velocity
}

TEST_CASE("Boid Update with max_speed") {
    boids::Vec3 pos(0.0, 0.0, 0.0);
    boids::Vec3 vel(1.0, 1.0, 1.0);
    boids::Boid b(pos, vel);

    boids::Vec3 delta_v(0.5, 0.5, 0.5);
    double max_speed = 2.0;
    
    b.updateBoid(delta_v, max_speed);

    // Ensure the velocity norm is clamped to max_speed
    CHECK(b.velocity().norm() == doctest::Approx(max_speed));
    CHECK(b.velocity() == boids::Vec3(1.1547, 1.1547, 1.1547)); // Same normalized value as above
    // Ensure the position is updated based on the velocity
    CHECK(b.position() == boids::Vec3(1.1547, 1.1547, 1.1547));
}

TEST_CASE("Border Handling: Toroidal and Bouncing") {
    boids::Vec3 screen(10.0, 10.0, 10.0);

    // Test 1: Toroidal wrapping
    boids::Boid toroidalBoid(boids::Vec3(11.0, -1.0, 5.0), boids::Vec3(1.0, 1.0, 1.0));
    boids::border(screen, true, toroidalBoid);

    CHECK(toroidalBoid.position() == boids::Vec3(1.0, 9.0, 5.0));  // x and y should wrap
    CHECK(toroidalBoid.velocity() == boids::Vec3(1.0, 1.0, 1.0));  // velocity should stay the same

    // Test 2: Bouncing off the walls
    boids::Boid bouncingBoid(boids::Vec3(11.0, 11.0, -1.0), boids::Vec3(1.0, 1.0, -2.0));
    boids::border(screen, false, bouncingBoid);

    CHECK(bouncingBoid.position() == boids::Vec3(10.0, 10.0, 0.0));  // Should clamp to screen size
    CHECK(bouncingBoid.velocity() == boids::Vec3(-0.9, -0.9, 1.8));  // Should invert and reduce velocity
}

TEST_CASE("vecDistance and distance: Toroidal and Non-Toroidal") {
    boids::Vec3 width(10.0, 10.0, 10.0);
    boids::Vec3 a(1.0, 1.0, 1.0);
    boids::Vec3 b(9.0, 9.0, 9.0);

    // Test 1: Non-Toroidal distance
    CHECK(boids::vecDistance(false, a, b, width) == boids::Vec3(-8.0, -8.0, -8.0));
    CHECK(boids::distance(false, a, b, width) == doctest::Approx(13.8564));

    // Test 2: Toroidal distance
    CHECK(boids::vecDistance(true, a, b, width) == boids::Vec3(2.0, 2.0, -8.0));
    CHECK(boids::distance(true, a, b, width) == doctest::Approx(8.544));
}

TEST_CASE("mean and stdDev: Statistical functions") {
    std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};

    // Test 1: Mean
    CHECK(boids::mean(values) == doctest::Approx(3.0));

    // Test 2: Standard Deviation
    double mean_value = boids::mean(values);
    CHECK(boids::stdDev(values, mean_value) == doctest::Approx(1.41421356));
}

TEST_CASE("maintainHeight: Height adjustment") {
    boids::Boid boid(boids::Vec3(1.0, 1.0, 5.0), boids::Vec3(0.0, 0.0, 1.0));

    // Test: Boid is moving away from the target height
    boids::Vec3 correction = boids::maintainHeight(boid, 3.0, 2.0);
    CHECK(correction.z() == doctest::Approx(-1.0));  // Moving down to the target height
}

TEST_CASE("Global Variables Initialization") {
    boids::GlobalVariables global_vars;

    CHECK(global_vars.screen == boids::Vec3(600, 300, 300));
    CHECK(global_vars.wind == boids::Vec3(0, 0, 0));
    CHECK(global_vars.toroidal_bool == false);
}

TEST_CASE("Predator Variables Initialization") {
    boids::PredatorVariables predator_vars;

    CHECK(predator_vars.attack_range == 300);
    CHECK(predator_vars.attack_speed == 1.1);
}

TEST_CASE("Swarm Variables Initialization") {
    boids::SwarmVariables swarm_vars;

    CHECK(swarm_vars.size == 100);
    CHECK(swarm_vars.wingspan == 2);
    CHECK(swarm_vars.max_speed == 1);
    CHECK(swarm_vars.min_distance == 30);
    CHECK(swarm_vars.sight_distance == 150);

    CHECK(swarm_vars.separation_factor == 0.05);
    CHECK(swarm_vars.cohesion_factor == 0.00005);
    CHECK(swarm_vars.alignment_factor == 0.005);
    CHECK(swarm_vars.fear_factor == 0.05);
}

TEST_CASE("Conversion Factors Initialization") {
    boids::ConversionFactors conversion_factors;

    CHECK(conversion_factors.speed_k == 100);
    CHECK(conversion_factors.separation_k == 1000);
    CHECK(conversion_factors.cohesion_k == 1000000);
    CHECK(conversion_factors.alignment_k == 10000);
    CHECK(conversion_factors.fear_k == 1000);
}

TEST_CASE("Swarm Initialization") {
    boids::GlobalVariables global_vars;
    boids::SwarmVariables swarm_vars;
    boids::Boid predator;

    boids::Swarm swarm(global_vars, swarm_vars, &predator);

    CHECK(swarm.size() == swarm_vars.size);
    CHECK(swarm.wingspan() == swarm_vars.wingspan);
    CHECK(swarm.max_speed() == swarm_vars.max_speed);
    CHECK(swarm.min_distance() == swarm_vars.min_distance);
    CHECK(swarm.sight_distance() == swarm_vars.sight_distance);
}

TEST_CASE("Swarm Boid Access") {
    boids::GlobalVariables global_vars;
    boids::SwarmVariables swarm_vars;
    boids::Boid predator;

    boids::Swarm swarm(global_vars, swarm_vars, &predator);

    CHECK_THROWS_AS(swarm[-1], std::out_of_range);
    CHECK_THROWS_AS(swarm[swarm.size()], std::out_of_range);
    CHECK_NOTHROW(swarm[0]);
}

TEST_CASE("Swarm Update") {
    boids::GlobalVariables global_vars;
    boids::SwarmVariables swarm_vars;
    boids::Boid predator;

    boids::Swarm swarm(global_vars, swarm_vars, &predator);

    CHECK_NOTHROW(swarm.updateSwarm());
}

