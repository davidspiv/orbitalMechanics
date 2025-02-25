#include "../include/io.h"
#include "../include/struct.h"
#include <iostream>
#include <math.h>

float getAccel(float pos)
{
   const float G = 6.674e-11; // gravitational constant
   const float M = 1.9891e30; //[kg] mass of sun
   const float m = 5.9722e24; //[kg] mass of earth
   const float r = 1.4959e11; //[meters] distance between sun and earth

   return G * (M + m) * pos / pow(r, 3);
}

int main()
{
   const Planet earth = {"earth",   1.00000011, 0.01671022, 0.00005,
                         -11.26064, 102.94719,  357.51716,  365.259636};

   Cord acc{0, 0, 0};
   Cord vel{0, 29780, 0};
   Cord pos{1.4959e11, 0, 0};

   Cord velocities[365];
   velocities[0] = vel;

   for (size_t i = 0; i < 365; i++)
   {
      acc.x = getAccel(pos.x);
      acc.y = getAccel(pos.y);
      acc.z = getAccel(pos.z);
   }

   print(pos.x);
}
