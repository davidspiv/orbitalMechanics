#ifndef IO_H
#define IO_H

#include "../include/struct.h"
#include <fstream>
#include <iostream>

template <typename T> void print(T output, bool carriageReturn = 1)
{
   if (carriageReturn)
   {
      std::cout << output << std::endl;
      return;
   }

   std::cout << output << std::flush;
}

// no strings
template <typename T> T getInput(const std::string &prompt)
{
   T input;
   std::cout << prompt;
   std::cin >> input;

   while (!std::cin || std::cin.peek() != '\n')
   {
      std::cin.clear();
      std::cin.ignore(100, '\n');
      std::cout << "Invalid" << std::endl;
      std::cout << prompt;
      std::cin >> input;
   }

   std::cin.get();
   return input;
}

// expose either input or output stream
template <typename T> bool openFile(T &fileStream, const std::string &fileName)
{
   fileStream.open(fileName);
   if (!fileStream)
   {
      return false;
   }

   return true;
}

// TODO: use SFINAE to restrict types to those writable to output stream
template <typename T>
bool writeToFile(const T &output, const std::string &fileName)
{
   std::ofstream outputStream(fileName);

   if (!outputStream)
   {
      return false;
   }

   outputStream << output;
   outputStream.close();

   return true;
}

std::string getString(const std::string &prompt);
void printMenu();
void printHistory(const Waypoint *waypoints, size_t numInputs);
int getMenuChoice(int maxChoice);
Date getDate();
std::string toLowercase(std::string input);
int getPlanetIndex();
int getVelocityAsMph();

#endif
