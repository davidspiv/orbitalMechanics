#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

const double G = 6.674e-11;      // Gravitational constant [m^3 kg^-1 s^-2]
const double M_sun = 1.9891e30;  // Mass of the sun [kg]

struct Vec3 {
  double x, y, z;
  Vec3 operator+(const Vec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
  }
  Vec3 operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }
  Vec3& operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
};

struct PlanetState {
  Vec3 position;
  Vec3 velocity;
  double mass;
};

Vec3 calcAcceleration(const PlanetState& p1, const PlanetState& p2) {
  Vec3 r = {p2.position.x - p1.position.x, p2.position.y - p1.position.y,
            p2.position.z - p1.position.z};
  double distSq = r.x * r.x + r.y * r.y + r.z * r.z;
  double dist = std::sqrt(distSq);
  double force =
      G * p2.mass /
      (distSq * dist);  // F = G * m1 * m2 / r^2, applied as accel = F / m1
  return r * force;
}

PlanetState rungeKuttaStep(const PlanetState& planet,
                           const std::vector<PlanetState>& planets, double dt) {
  auto computeAcceleration = [&](const PlanetState& p) -> Vec3 {
    Vec3 acc = {0, 0, 0};
    for (const auto& other : planets) {
      if (&p != &other) acc += calcAcceleration(p, other);
    }
    if (std::isnan(acc.x)) {
      acc.x = 0;
    }

    if (std::isnan(acc.y)) {
      acc.y = 0;
    }

    if (std::isnan(acc.z)) {
      acc.z = 0;
    }
    return acc;
  };

  Vec3 k1v = computeAcceleration(planet) * dt;
  Vec3 k1r = planet.velocity * dt;

  Vec3 k2v = computeAcceleration({planet.position + k1r * 0.5,
                                  planet.velocity + k1v * 0.5, planet.mass}) *
             dt;
  Vec3 k2r = (planet.velocity + k1v * 0.5) * dt;

  Vec3 k3v = computeAcceleration({planet.position + k2r * 0.5,
                                  planet.velocity + k2v * 0.5, planet.mass}) *
             dt;
  Vec3 k3r = (planet.velocity + k2v * 0.5) * dt;

  Vec3 k4v = computeAcceleration(
                 {planet.position + k3r, planet.velocity + k3v, planet.mass}) *
             dt;
  Vec3 k4r = (planet.velocity + k3v) * dt;

  PlanetState newPlanet = planet;
  newPlanet.velocity += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  newPlanet.position += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return newPlanet;
}

int main() {
  // Example: Sun and Earth
  std::vector<PlanetState> planets = {
      {{0, 0, 0}, {0, 0, 0}, M_sun},                 // Sun
      {{1.4959e11, 0, 0}, {0, 29780, 0}, 5.9722e24}  // Earth
  };

  double dt = 86400;  // 1 day in seconds
  int steps = 365;    // Simulate 1 year

  for (int i = 0; i < steps; i++) {
    std::vector<PlanetState> newStates;
    for (const auto& planet : planets) {
      newStates.push_back(rungeKuttaStep(planet, planets, dt));
    }
    planets = newStates;
  }
  const double gX = planets[1].position.x;
  const double gY = planets[1].position.y;
  const double gZ = planets[1].position.z;
  
  const double distance = sqrt(gX * gX + gY * gY + gZ * gZ);

  std::cout << "Day " << steps
            << " Earth distance to sun AU: " +
                   std::to_string(distance / 1.496e+11)
            << std::endl;

  return 0;
}
