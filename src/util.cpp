#include <math.h>
#include <string>

// converts degrees to a value between 0 and 360
double normalizeDegrees(double x)
{
   return x - floor(x / 360.0) * 360.0;
}

// converts degrees to radians
double toRadians(double degrees)
{
   return degrees * (M_PI / 180.0);
}

// will convert a double to a comma separated value truncated to two decimal
// places
std::string formatDouble(double yearsAsDouble)
{
   std::string yearsAsString = std::to_string(yearsAsDouble);
   yearsAsString = yearsAsString.substr(0, yearsAsString.length() - 4);

   for (int i = yearsAsString.length() - 6; i > 0; i = i - 3)
   {
      yearsAsString.insert(i, ",");
   }

   return yearsAsString;
}
