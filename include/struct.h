#ifndef STRUCT_H
#define STRUCT_H

#include <string>

struct Planet
{
   std::string name;
   double semiMajorAxis;
   double eccentricity;
   double orbitalInclination;
   double longitudeOfAscendingNode;
   double longitudeOfPerihelion;
   double meanAnomaly;
   double period;
};

struct Cord
{
   double x;
   double y;
   double z;
};

struct Date
{
   int month;
   int day;
   int year;
   double universalTime;
};

struct Waypoint
{
   Date date;
   std::string planetName;
   double geocentricDistance;
};

#endif
