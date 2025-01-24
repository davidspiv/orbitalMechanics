
#include <math.h>

#include "../include/io.h"

using namespace std;

struct newPlanet
{
   string name;
   double semiMajorAxis;
   double mass;
};

int main()
{
   newPlanet mercury;
   mercury.semiMajorAxis = 0.38709893;
   // mercury.eccentricity = 0.20563069;
   mercury.mass = 3.285e+23;

   // const double areaElipse =
   //     pow(mercury.semiMajorAxis, 2) * sqrt(1 - pow(mercury.eccentricity,
   //     2));

   // Kepler's Third Law: The square of the period of an object's orbit is
   // proportional to the cube of its semi-major axis

   const double auToM = 1.496e+11;
   // const double secondsToYears = 1.15741e-5;
   const double sunMass = 1.9891e+30;
   const double G = 6.673e-11;

   const double proportionalityConstant =
       (4 * pow(M_PI, 2)) / (G * (mercury.mass + sunMass));

   const double period =
       sqrt(proportionalityConstant * pow(mercury.semiMajorAxis * auToM, 3));

   print(period);

   return 0;
}
