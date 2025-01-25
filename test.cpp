#include "./include/io.h"
#include "./include/struct.h"
#include <fstream>
#include <iostream>

std::string getValueFromJSONLine(std::string line)
{
   const int startIndex = line.rfind(':', line.length() - 1) + 2;
   const int valueLength = line.length() - startIndex - 1;
   print(line.substr(startIndex, valueLength));
   return line.substr(startIndex, valueLength);
}

// reads planets.json into a dynamically allocated array of planet structs
Planet *populatePlanets()
{
   const std::string firstKey = "\"name\": \"";
   const size_t numberOfPlanets = 8;
   Planet *planets = new Planet[numberOfPlanets]{{"", 0, 0, 0, 0, 0, 0, 0}};
   size_t planetIndex = 0;
   std::fstream fileStream;
   std::string line;

   openFile(fileStream, "planets.json");

   while (std::getline(fileStream, line))
   {
      const int objectStart = line.find(firstKey);

      if (objectStart > 0)
      {
         std::string name = getValueFromJSONLine(line);
         planets[planetIndex].name = name.substr(1, name.length() - 2);

         std::getline(fileStream, line);
         planets[planetIndex].semiMajorAxis =
             std::stod(getValueFromJSONLine(line));

         std::getline(fileStream, line);
         planets[planetIndex].eccentricity =
             std::stod(getValueFromJSONLine(line));

         std::getline(fileStream, line);
         planets[planetIndex].orbitalInclination =
             std::stod(getValueFromJSONLine(line));

         std::getline(fileStream, line);
         planets[planetIndex].longitudeOfAscendingNode =
             std::stod(getValueFromJSONLine(line));

         std::getline(fileStream, line);
         planets[planetIndex].longitudeOfPerihelion =
             std::stod(getValueFromJSONLine(line));

         std::getline(fileStream, line);
         planets[planetIndex].meanAnomaly =
             std::stod(getValueFromJSONLine(line));

         std::getline(fileStream, line);
         planets[planetIndex].period = std::stod(getValueFromJSONLine(line));

         planetIndex++;
         if (planetIndex > numberOfPlanets)
            break;
      }
   }
   return planets;
}

int main()
{
   Planet *planets = populatePlanets();
   for (size_t i = 0; i < 8; i++)
   {
      print(planets[i].name);
      print(planets[i].longitudeOfPerihelion);
   }
}
