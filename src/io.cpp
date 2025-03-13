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

// gets date std::string from user input
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

// returns value sans quotes regardless if its a string or a number
std::string getValueFromJSONLine(std::string line)
{
   const size_t startIndex = line.rfind(':', line.length() - 1) + 2;
   const size_t valueLength = line.length() - startIndex;
   std::string value = line.substr(startIndex, valueLength);

   // remove quotes from json string values;
   if (line.find(": \"") == std::string::npos)
   {
      return value;
   }

   // ignore trailing commas
   const size_t endIndex = value.rfind('"') - 1;
   return value.substr(1, endIndex);
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
         planets[planetIndex].name = getValueFromJSONLine(line);

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
