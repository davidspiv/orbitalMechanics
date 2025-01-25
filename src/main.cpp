#include <math.h>

#include <iomanip>
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
float calcDistance(const Planet &planet, float days)
{
   const Planet earth = {"earth",   1.00000011, 0.01671022, 0.00005,
                         -11.26064, 102.94719,  357.51716,  365.259636};

   Cord heliocentricCordPlanet = calcHeliocentricCord(planet, days);
   const Cord heliocentricCordEarth = calcHeliocentricCord(earth, days);

   // geocentric 3d cartesian coordinates
   const double gX = heliocentricCordEarth.x - heliocentricCordPlanet.x;
   const double gY = heliocentricCordEarth.y - heliocentricCordPlanet.y;
   const double gZ = heliocentricCordEarth.z - heliocentricCordPlanet.z;

   const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));

   return distance;
}

int main()
{
   const Planet *planets = populatePlanets();
   const Date date = getDate();
   const float days = calcDaysSinceEpoch(date);

   for (size_t i = 0; i < 8; i++)
   {
      const string name = planets[i].name;
      const float distance = calcDistance(planets[i], days);

      cout << name << std::setw(10 - name.length()) << " : " << distance
           << endl;
   }

   delete[] planets;
   planets = nullptr;

   return 0;
}
