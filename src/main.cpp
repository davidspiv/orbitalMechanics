#include <math.h>

#include <iostream>
#include <string>

#include "../include/io.h"
#include "../include/struct.h"
#include "../include/util.h"

using namespace std;

// finds difference in days between date argument and the J2000 epoch
double calcDaysSinceEpoch(const Date &date)
{
   const int month = date.month;
   const int day = date.day;
   const int year = date.year;
   const int universalTime = date.universalTime;

   // intentional integer division
   double totalDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                      3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                      275 * month / 9 + day - 730515;
   totalDays = totalDays + universalTime / 24.0;

   return totalDays - 1;
}

// populates a dynamically allocated array with Planet structs and returns the
// owner pointer
Planet *populatePlanets()
{
   Planet mercury;
   mercury.name = "mercury";
   mercury.semiMajorAxis = 0.38709893;
   mercury.eccentricity = 0.20563069;
   mercury.orbitalInclination = 7.00487;
   mercury.longitudeOfAscendingNode = 48.33167;
   mercury.longitudeOfPerihelion = 77.45645;
   mercury.meanAnomaly = 174.796;
   mercury.period = 87.969;

   Planet venus;
   venus.name = "venus";
   venus.semiMajorAxis = 0.72333199;
   venus.eccentricity = 0.00677323;
   venus.orbitalInclination = 3.39471;
   venus.longitudeOfAscendingNode = 76.68069;
   venus.longitudeOfPerihelion = 131.53298;
   venus.meanAnomaly = 50.45;
   venus.period = 224.7008;

   Planet earth;
   earth.name = "earth";
   earth.semiMajorAxis = 1.00000011;
   earth.eccentricity = 0.01671022;
   earth.orbitalInclination = 0.00005;
   earth.longitudeOfAscendingNode = -11.26064;
   earth.longitudeOfPerihelion = 102.94719;
   earth.meanAnomaly = 357.51716;
   earth.period = 365.259636;

   Planet mars;
   mars.name = "mars";
   mars.semiMajorAxis = 1.52366231;
   mars.eccentricity = 0.09341233;
   mars.orbitalInclination = 1.85061;
   mars.longitudeOfAscendingNode = 49.57854;
   mars.longitudeOfPerihelion = 336.04084;
   mars.meanAnomaly = 19.387;
   mars.period = 686.9957;

   Planet jupiter;
   jupiter.name = "jupiter";
   jupiter.semiMajorAxis = 5.20336301;
   jupiter.eccentricity = 0.04839266;
   jupiter.orbitalInclination = 1.30530;
   jupiter.longitudeOfAscendingNode = 100.55615;
   jupiter.longitudeOfPerihelion = 14.75385;
   jupiter.meanAnomaly = 20.020;
   jupiter.period = 11.862;

   Planet saturn;
   saturn.name = "saturn";
   saturn.semiMajorAxis = 9.53707032;
   saturn.eccentricity = 0.05415060;
   saturn.orbitalInclination = 2.48446;
   saturn.longitudeOfAscendingNode = 113.71504;
   saturn.longitudeOfPerihelion = 92.43194;
   saturn.meanAnomaly = 317.020;
   saturn.period = 29.4475;

   Planet uranus;
   uranus.name = "uranus";
   uranus.semiMajorAxis = 19.19126393;
   uranus.eccentricity = 0.04716771;
   uranus.orbitalInclination = 0.76986;
   uranus.longitudeOfAscendingNode = 74.22988;
   uranus.longitudeOfPerihelion = 170.96424;
   uranus.meanAnomaly = 142.238600;
   uranus.period = 84.011;

   Planet neptune;
   neptune.name = "neptune";
   neptune.semiMajorAxis = 30.06896348;
   neptune.eccentricity = 0.00858587;
   neptune.orbitalInclination = 1.76917;
   neptune.longitudeOfAscendingNode = 131.72169;
   neptune.longitudeOfPerihelion = 44.97135;
   neptune.meanAnomaly = 259.883;
   neptune.period = 164.79;

   Planet *planets = new Planet[8]{mercury, venus,  earth,  mars,
                                   jupiter, saturn, uranus, neptune};

   return planets;
}

// Numerical approximation of Eccentric Anomaly (E) using the Newton-Raphson
// method
double calcEccentricAnomaly(double eccentricity, double normalizedMeanAnomaly)
{
   const double e = eccentricity;
   const double M = normalizedMeanAnomaly;

   auto isConverging = [](int count) { return count < 19; };

   // the inverse of the standard form of Kepler's equation
   double E = M + e * sin(M) * (1 + e * cos(M));

   double delta;
   int iterationCount = 0;

   do
   {
      const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
      delta = abs(E1 - E);
      E = E1;
      iterationCount++;
   } while (delta >= 0.00001 && isConverging(iterationCount));

   // failsafe, should never happen with current planet selection
   if (!isConverging(iterationCount))
   {
      print("calcEccentricAnomaly() failed. unable to converge.");
      print("delta: " + to_string(delta));
      exit(1);
   }

   return E;
}

double getNormalizedMeanAnomaly(Planet planet, float daysSinceEpoch)
{
   const double meanMotion = 360.0 / planet.period;

   return normalizeDegrees(planet.meanAnomaly + meanMotion * daysSinceEpoch);
}

// calculates the heliocentric coordinates of the planet at the specified time
Cord calcHeliocentricCord(const Planet &planet, float daysSinceEpoch)
{
   const double normalizedMeanAnomaly =
       getNormalizedMeanAnomaly(planet, daysSinceEpoch);

   // orbital elements normalized to J2000
   const double a = planet.semiMajorAxis;
   const double e = planet.eccentricity;
   const double o = toRadians(planet.longitudeOfAscendingNode);
   const double p = toRadians(planet.longitudeOfPerihelion);
   const double i = toRadians(planet.orbitalInclination);

   // normalized to specified date
   const double M = toRadians(normalizedMeanAnomaly);
   const double E = calcEccentricAnomaly(e, M);

   // position in 2d orbital plane
   const double xv = a * (cos(E) - e);
   const double yv = a * (sqrt(1.0 - e * e) * sin(E));

   // The True Anomaly (v) is the final angle we need to define the position
   // of a satellite in orbit, the other two being Eccentric Anomaly (E) and
   // Mean Anomaly (M).
   const double v = atan2(yv, xv);

   // The radius vector (r) is the distance of the satellite to the focal point
   // of the ellipse, in this case the sun.
   const double r = sqrt(xv * xv + yv * yv);

   // heliocentric 3d cartesian coordinates
   const double xh =
       r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
   const double yh =
       r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
   const double zh = r * (sin(v + p - o) * sin(i));

   return {xh, yh, zh};
}

// creates a Waypoint struct that includes inputs and the results attributed to
// one travel entry. Acts as a controller
Waypoint createWaypoint(const Planet *&planets)
{
   const Date date = getDate();
   const int planetIndex = getPlanetIndex();

   const float days = calcDaysSinceEpoch(date);

   const Planet planet = planets[planetIndex];
   const Planet earth = planets[2];

   Cord heliocentricCordPlanet = calcHeliocentricCord(planet, days);
   const Cord heliocentricCordEarth = calcHeliocentricCord(earth, days);

   // geocentric 3d cartesian coordinates
   const double gX = heliocentricCordEarth.x - heliocentricCordPlanet.x;
   const double gY = heliocentricCordEarth.y - heliocentricCordPlanet.y;
   const double gZ = heliocentricCordEarth.z - heliocentricCordPlanet.z;

   const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

   return {date, planet.name, distance};
}

int main()
{
   const Planet *planets = populatePlanets();
   Waypoint waypoint = createWaypoint(planets);
   cout << "GEOCENTRIC DISTANCE: " << waypoint.geocentricDistance << endl;

   delete[] planets;
   planets = nullptr;

   return 0;
}
