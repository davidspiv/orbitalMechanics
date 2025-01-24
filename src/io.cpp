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

// shows menu choices that correspond to integer constants
void printMenu()
{
   print(R"(
1. Add waypoint
2. History
3. Total
4. Quit)");
}

// loops through data arrays and prints out past inputs and results
void printHistory(const Waypoint *waypoints, size_t numInputs)
{
   std::cout << std::endl
             << std::setw(11) << std::left << "Date" << std::setw(9)
             << "Planet" << std::setw(13) << "MPH" << std::setw(9) << "AU"
             << "Years" << std::endl;
   std::cout << std::setw(11) << std::left << "----" << std::setw(9)
             << "------" << std::setw(13) << "---" << std::setw(9) << "--"
             << "-----" << std::endl;

   // detect intentional integer wrap around so I can keep using unsigned type
   for (size_t i = (numInputs - 1); i < numInputs; i--)
   {
      const std::string month = std::to_string(waypoints[i].date.month);
      const std::string day = std::to_string(waypoints[i].date.day);
      const std::string year = std::to_string(waypoints[i].date.year);

      const std::string date = month + '/' + day + '/' + year;

      std::cout << std::setw(11) << std::left << date << std::setw(9)
                << waypoints[i].planetName << std::setw(13)
                << waypoints[i].velocityAsMph << std::setw(9)
                << waypoints[i].geocentricDistance << waypoints[i].timeAsYears
                << std::endl;
   }
}

// gets menu option and returns it
int getMenuChoice(int maxChoice)
{
   int menuOption = 0;

   printMenu();
   menuOption = getInput<int>("Choose option: ");

   while (menuOption <= 0 || menuOption > maxChoice)
   {
      print("Must choose number from menu options.");
      printMenu();
      menuOption = getInput<int>("Choose option: ");
   }

   return menuOption;
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

// gets a user input is miles per hour and returns it
int getVelocityAsMph()
{
   int velocityAsMph = getInput<int>("Enter miles per hour: ");

   while (velocityAsMph <= 0)
   {
      print("Must be greater than 0.");
      velocityAsMph = getInput<int>("Enter miles per hour: ");
   }

   return velocityAsMph;
}
