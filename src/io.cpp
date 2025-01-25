#include <iomanip>
#include <iostream>
#include <string>

#include "../include/io.h"
#include "../include/struct.h"

std::string getString(const std::string &prompt)
{
   std::string input = "";
   std::cout << prompt;
   getline(std::cin, input);

   while (input == "")
   {
      std::cout << "No characters entered." << std::endl;
      std::cout << prompt;
      getline(std::cin, input);
   }

   return input;
}

// gets date std::string from user input, more lenient than prompt suggests
Date getDate()
{
   int month = 0;
   int day = 0;
   int year = 0;

   bool isFormatted = false;

   std::cout << std::endl;

   do
   {
      int deliminatorCount = 0;
      std::string numAsString = "0";

      const std::string date = getString("Enter a date (MM/DD/YYYY): ");

      for (const char character : date)
      {
         if (character == '/')
         {
            deliminatorCount += 1;

            if (deliminatorCount == 1)
            {
               month = stoi(numAsString);
               numAsString = "0";
            }
            else if (deliminatorCount == 2)
            {
               day = stoi(numAsString);
               numAsString = "0";
            }
         }
         else
         {
            numAsString += character;
         }
      }

      year = stoi(numAsString);

      if (deliminatorCount == 2 && month > 0 && month <= 12 && day > 0 &&
          day <= 31)
      {
         isFormatted = true;
      }
      else
      {
         print("Date formatted incorrectly, try again");
      }

   } while (!isFormatted);

   return {month, day, year, 0.0};
}

// transform any uppercase letter to lowercase
std::string toLowercase(std::string input)
{
   for (char &character : input)
   {
      character = tolower(character);
   }

   return input;
}

// matches a user input with a list of celestial bodies
int getPlanetIndex()
{
   int planetIndex = -1;

   do
   {
      std::string name = getString("Enter a planet in our solar system: ");
      name = toLowercase(name);

      if (name.length() > 10)
      {
         print("Planet meanAnomaly must be within our solar system.");
      }
      else if (name.find("mercury") != std::string::npos)
      {
         planetIndex = 0;
      }
      else if (name.find("venus") != std::string::npos)
      {
         planetIndex = 1;
      }
      else if (name.find("mars") != std::string::npos)
      {
         planetIndex = 3;
      }
      else if (name.find("jupiter") != std::string::npos)
      {
         planetIndex = 4;
      }
      else if (name.find("saturn") != std::string::npos)
      {
         planetIndex = 5;
      }
      else if (name.find("uranus") != std::string::npos)
      {
         planetIndex = 6;
      }
      else if (name.find("neptune") != std::string::npos)
      {
         planetIndex = 7;
      }
      else
      {
         print("Planet must be within our solar system.");
      }
   } while (planetIndex == -1);

   return planetIndex;
}

// reads planets.json into a dynamically allocated array of planet structs
Planet *populatePlanets()
{
   Planet *planets = new Planet[8]{{"", 0, 0, 0, 0, 0, 0, 0}};
   size_t planetIndex = 0;
   const std::string searchString = "\"name\": \"";
   std::fstream fileStream;
   std::string line;

   openFile(fileStream, "planets.json");

   while (std::getline(fileStream, line))
   {
      const int objectStart = line.find("\"name\": \"");

      if (objectStart > 0)
      {
         const double nameIndex = objectStart + searchString.length();

         planets[planetIndex].name =
             line.substr(nameIndex, line.length() - nameIndex - 2);

         std::getline(fileStream, line);
         planets[planetIndex].semiMajorAxis =
             std::stod(line.substr(objectStart + 17));

         std::getline(fileStream, line);
         planets[planetIndex].eccentricity =
             std::stod(line.substr(objectStart + 16));

         std::getline(fileStream, line);
         planets[planetIndex].orbitalInclination =
             std::stod(line.substr(objectStart + 22));

         std::getline(fileStream, line);
         planets[planetIndex].longitudeOfAscendingNode =
             std::stod(line.substr(objectStart + 28));

         std::getline(fileStream, line);
         planets[planetIndex].longitudeOfPerihelion =
             std::stod(line.substr(objectStart + 25));

         std::getline(fileStream, line);
         planets[planetIndex].meanAnomaly =
             std::stod(line.substr(objectStart + 15));

         std::getline(fileStream, line);
         planets[planetIndex].period =
             std::stod(line.substr(objectStart + 10));

         //  std::cout << planets[planetIndex].name << std::endl;
         //  std::cout << planets[planetIndex].semiMajorAxis << std::endl;
         //  std::cout << planets[planetIndex].eccentricity << std::endl;
         //  std::cout << planets[planetIndex].orbitalInclination << std::endl;
         //  std::cout << planets[planetIndex].longitudeOfAscendingNode
         //            << std::endl;
         //  std::cout << planets[planetIndex].longitudeOfPerihelion <<
         //  std::endl; std::cout << planets[planetIndex].meanAnomaly <<
         //  std::endl; std::cout << planets[planetIndex].period << std::endl;

         planetIndex++;
      }
   }
   return planets;
}
