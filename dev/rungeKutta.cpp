#include <cmath>
#include <iostream>

#include "../include/io.h"

double calcVelocity(double x, double y)
{
   return -2 * y + pow(x, 3) * pow(M_E, -2 * x);
}

int main()
{
   const double h = 0.1;
   double x = 0;
   double y = 1;

   for (size_t i = 0; i < 2; i++)
   {
      const double k1 = calcVelocity(x, y);
      const double k2 = calcVelocity(x + h / 2, y + h * k1 / 2);
      const double k3 = calcVelocity(x + h / 2, y + h * k2 / 2);
      const double k4 = calcVelocity(x + h, y + h * k3);

      y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;

      print(y);
   }
}
