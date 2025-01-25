#ifndef IO_H
#define IO_H

#include "../src/io.tpp"
#include "struct.h"
#include <iostream>

template <typename T> void print(T output, bool carriageReturn);

// no strings
template <typename T> T getInput(const std::string &prompt);

// expose either input or output stream
template <typename T>
bool openFile(T &fileStream, const std::string &fileName);

// TODO: use SFINAE to restrict types to those writable to output stream
template <typename T>
bool writeToFile(const T &output, const std::string &fileName);

std::string getString(const std::string &prompt);

Date getDate();

std::string toLowercase(std::string input);

int getPlanetIndex();

Planet *populatePlanets();

#endif
