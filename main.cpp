
#include <math.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "helpers/io.h"

using namespace std;

struct Planet {
  string name;
  double semiMajorAxis;
  double eccentricity;
  double orbitalInclination;
  double longitudeOfAscendingNode;
  double longitudeOfPerihelion;
  double meanAnomaly;
  double period;
};

struct Cord {
  double x;
  double y;
  double z;
};

struct Date {
  int month;
  int day;
  int year;
  double ut;
};

struct Waypoint {
  Date date;
  string planetName;
  double velocityAsMph;
  double geocentricDistance;
  double timeAsYears;
};

// transform any uppercase letter to lowercase
string toLowercase(string input) {
  for (char &character : input) {
    character = tolower(character);
  }

  return input;
}

// allocates a larger but otherwise identical array
template <typename T>
void expandArray(T *&arr, size_t &maxInputs) {
  const size_t step = 5;

  T *copyArr = new T[maxInputs + step];

  for (size_t i = 0; i < maxInputs; i++) {
    copyArr[i] = arr[i];
  }

  delete[] arr;
  arr = copyArr;

  maxInputs += step;
}

// ensures degrees is within the standard circle range
double normalizeDegrees(double x) { return x - floor(x / 360.0) * 360.0; }

// converts degrees to radians
double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

// finds difference in days between date argument and the J2000 epoch
double calcDaysSinceEpoch(const Date &date) {
  const int month = date.month;
  const int day = date.day;
  const int year = date.year;
  const int ut = date.ut;

  // intentional integer division
  double totalDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                     3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                     275 * month / 9 + day - 730515;
  totalDays = totalDays + ut / 24.0;

  return totalDays - 1;
}

// will convert a double to a comma separated value truncated to two decimal
// places
string formatDouble(double yearsAsDouble) {
  string yearsAsString = to_string(yearsAsDouble);
  yearsAsString = yearsAsString.substr(0, yearsAsString.length() - 4);

  for (int i = yearsAsString.length() - 6; i > 0; i = i - 3) {
    yearsAsString.insert(i, ",");
  }

  return yearsAsString;
}

// converts time format if needed and display a border around the output
string formatTimeResult(const string &label, double years) {
  string formattedTime = label + ": ";
  string resultBorder = "";

  if (years == 0) {
    formattedTime += "None";
  } else if (years == 1) {
    formattedTime += "1 year";
  } else if (years < 1) {
    formattedTime += to_string(years) + " of a year";
  } else if (years > 100000) {
    formattedTime += formatDouble(years / 1000) + " millennia";
  } else {
    formattedTime += formatDouble(years) + " years";
  }

  // border should be same length as result
  resultBorder = string(formattedTime.length(), '-');

  return "\n" + resultBorder + "\n" + formattedTime + "\n" + resultBorder;
}

// outputs value to console
void print(const string &output, bool carriageReturn = 1) {
  if (carriageReturn) {
    cout << output << endl;
    return;
  }
  cout << output << flush;
}

// shows menu choices that correspond to integer constants
void printMenu() {
  print(R"(
1. Add waypoint
2. History
3. Total
4. Quit)");
}

// loops through data arrays and prints out past inputs and results
void printHistory(const Waypoint *waypoints, size_t numInputs) {
  cout << endl
       << setw(11) << left << "Date" << setw(9) << "Planet" << setw(13) << "MPH"
       << setw(9) << "AU" << "Years" << endl;
  cout << setw(11) << left << "----" << setw(9) << "------" << setw(13) << "---"
       << setw(9) << "--" << "-----" << endl;

  // detect intentional integer wrap around so I can keep using unsigned type
  for (size_t i = (numInputs - 1); i < numInputs; i--) {
    const string month = to_string(waypoints[i].date.month);
    const string day = to_string(waypoints[i].date.day);
    const string year = to_string(waypoints[i].date.year);

    const string date = month + '/' + day + '/' + year;

    cout << setw(11) << left << date << setw(9) << waypoints[i].planetName
         << setw(13) << waypoints[i].velocityAsMph << setw(9)
         << waypoints[i].geocentricDistance << waypoints[i].timeAsYears << endl;
  }
}

// gets menu option and returns it
int getMenuChoice(int maxChoice) {
  int menuOption = 0;

  printMenu();
  menuOption = getInput<int>("Choose option: ");

  while (menuOption <= 0 || menuOption > maxChoice) {
    print("Must choose number from menu options.");
    printMenu();
    menuOption = getInput<int>("Choose option: ");
  }

  return menuOption;
}

// gets date string from user input, more lenient than prompt suggests
Date getDate() {
  int month = 0;
  int day = 0;
  int year = 0;

  bool isFormatted = false;

  cout << endl;

  do {
    int deliminatorCount = 0;
    string numAsString = "0";

    const string date = getString("Enter a date (MM/DD/YYYY): ");

    for (const char character : date) {
      if (character == '/') {
        deliminatorCount += 1;

        if (deliminatorCount == 1) {
          month = stoi(numAsString);
          numAsString = "0";
        } else if (deliminatorCount == 2) {
          day = stoi(numAsString);
          numAsString = "0";
        }
      } else {
        numAsString += character;
      }
    }

    year = stoi(numAsString);

    if (deliminatorCount == 2 && month > 0 && month <= 12 && day > 0 &&
        day <= 31) {
      isFormatted = true;
    } else {
      print("Date formatted incorrectly, try again");
    }

  } while (!isFormatted);

  return {month, day, year, 0.0};
}

// matches a user input with a list of celestial bodies
int getPlanetIndex() {
  int planetIndex = -1;

  do {
    string name = getString("Enter a planet in our solar system: ");
    name = toLowercase(name);

    if (name.length() > 10) {
      print("Planet meanAnomaly must be within our solar system.");
    } else if (name.find("mercury") != string::npos) {
      planetIndex = 0;
    } else if (name.find("venus") != string::npos) {
      planetIndex = 1;
    } else if (name.find("mars") != string::npos) {
      planetIndex = 3;
    } else if (name.find("jupiter") != string::npos) {
      planetIndex = 4;
    } else if (name.find("saturn") != string::npos) {
      planetIndex = 5;
    } else if (name.find("uranus") != string::npos) {
      planetIndex = 6;
    } else if (name.find("neptune") != string::npos) {
      planetIndex = 7;
    } else {
      print("Planet must be within our solar system.");
    }
  } while (planetIndex == -1);

  return planetIndex;
}

// gets a user input is miles per hour and returns it
int getVelocityAsMph() {
  int velocityAsMph = getInput<int>("Enter miles per hour: ");

  while (velocityAsMph <= 0) {
    print("Must be greater than 0.");
    velocityAsMph = getInput<int>("Enter miles per hour: ");
  }

  return velocityAsMph;
}

// populates a dynamically allocated array with Planet structs and returns the
// owner pointer
Planet *populatePlanets() {
  Planet mercury;
  mercury.name = "mercury";
  mercury.semiMajorAxis = 0.38709893;
  mercury.eccentricity = 0.20563069;
  mercury.orbitalInclination = 7.00487;
  mercury.longitudeOfAscendingNode = 48.33167;
  mercury.longitudeOfPerihelion = 77.45645;
  mercury.meanAnomaly = 174.796;
  mercury.period = 87.969;

  Planet venus;
  venus.name = "venus";
  venus.semiMajorAxis = 0.72333199;
  venus.eccentricity = 0.00677323;
  venus.orbitalInclination = 3.39471;
  venus.longitudeOfAscendingNode = 76.68069;
  venus.longitudeOfPerihelion = 131.53298;
  venus.meanAnomaly = 50.45;
  venus.period = 224.7008;

  Planet earth;
  earth.name = "earth";
  earth.semiMajorAxis = 1.00000011;
  earth.eccentricity = 0.01671022;
  earth.orbitalInclination = 0.00005;
  earth.longitudeOfAscendingNode = -11.26064;
  earth.longitudeOfPerihelion = 102.94719;
  earth.meanAnomaly = 357.51716;
  earth.period = 365.259636;

  Planet mars;
  mars.name = "mars";
  mars.semiMajorAxis = 1.52366231;
  mars.eccentricity = 0.09341233;
  mars.orbitalInclination = 1.85061;
  mars.longitudeOfAscendingNode = 49.57854;
  mars.longitudeOfPerihelion = 336.04084;
  mars.meanAnomaly = 19.387;
  mars.period = 686.9957;

  Planet jupiter;
  jupiter.name = "jupiter";
  jupiter.semiMajorAxis = 5.20336301;
  jupiter.eccentricity = 0.04839266;
  jupiter.orbitalInclination = 1.30530;
  jupiter.longitudeOfAscendingNode = 100.55615;
  jupiter.longitudeOfPerihelion = 14.75385;
  jupiter.meanAnomaly = 20.020;
  jupiter.period = 11.862;

  Planet saturn;
  saturn.name = "saturn";
  saturn.semiMajorAxis = 9.53707032;
  saturn.eccentricity = 0.05415060;
  saturn.orbitalInclination = 2.48446;
  saturn.longitudeOfAscendingNode = 113.71504;
  saturn.longitudeOfPerihelion = 92.43194;
  saturn.meanAnomaly = 317.020;
  saturn.period = 29.4475;

  Planet uranus;
  uranus.name = "uranus";
  uranus.semiMajorAxis = 19.19126393;
  uranus.eccentricity = 0.04716771;
  uranus.orbitalInclination = 0.76986;
  uranus.longitudeOfAscendingNode = 74.22988;
  uranus.longitudeOfPerihelion = 170.96424;
  uranus.meanAnomaly = 142.238600;
  uranus.period = 84.011;

  Planet neptune;
  neptune.name = "neptune";
  neptune.semiMajorAxis = 30.06896348;
  neptune.eccentricity = 0.00858587;
  neptune.orbitalInclination = 1.76917;
  neptune.longitudeOfAscendingNode = 131.72169;
  neptune.longitudeOfPerihelion = 44.97135;
  neptune.meanAnomaly = 259.883;
  neptune.period = 164.79;

  //  N = 131.7806_deg + 3.0173E-5_deg    * d
  // i =   1.7700_deg - 2.55E-7_deg      * d
  // w = 272.8461_deg - 6.027E-6_deg     * d
  // a = 30.05826     + 3.313E-8         * d
  // e = 0.008606     + 2.15E-9          * d
  // M = 260.2471_deg + 0.005995147_deg  * d

  Planet *planets = new Planet[8]{mercury, venus,  earth,  mars,
                                  jupiter, saturn, uranus, neptune};

  return planets;
}

// solves for time using the velocity equation
double calcYears(double distanceAsAU, double velocityAsMph) {
  const int HOURS_PER_YEAR = 8760;
  const double MILES_PER_AU = 9.296e+7;

  const double distanceAsMiles = distanceAsAU * MILES_PER_AU;

  return distanceAsMiles / velocityAsMph / HOURS_PER_YEAR;
}

// loops through the array and calculates each distance value, returning the sum
double calcTotalTime(Waypoint *&waypoints) {
  int validIndex = 0;
  double totalTime = 0;

  while (waypoints[validIndex].planetName != "" &&
         waypoints[validIndex].velocityAsMph != 0) {
    totalTime += waypoints[validIndex].timeAsYears;
    validIndex += 1;
  }

  return totalTime;
}

// formats total and displays to console
void printTotal(Waypoint *&waypoints) {
  double totalTime = calcTotalTime(waypoints);
  string totalTimeAsString = formatTimeResult("Total time", totalTime);

  print(totalTimeAsString);
}

// Numerical approximation of Eccentric Anomaly (E) using the Newton-Raphson
// method
double calcEccentricAnomaly(double eccentricity, double normalizedMeanAnomaly) {
  const double e = eccentricity;
  const double M = normalizedMeanAnomaly;

  auto isConverging = [](int count) { return count < 19; };

  // the inverse of the standard form of Kepler's equation
  double E = M + e * sin(M) * (1 + e * cos(M));

  double delta;
  int iterationCount = 0;

  do {
    const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
    delta = abs(E1 - E);
    E = E1;
    iterationCount++;
  } while (delta >= 0.00001 && isConverging(iterationCount));

  // failsafe, should never happen with current planet selection
  if (!isConverging(iterationCount)) {
    print("calcEccentricAnomaly() failed. unable to converge.");
    print("delta: " + to_string(delta));
    exit(1);
  }

  return E;
}

double getNormalizedMeanAnomaly(Planet planet, float daysSinceEpoch) {
  const double meanMotion = 360.0 / planet.period;

  return normalizeDegrees(planet.meanAnomaly + meanMotion * daysSinceEpoch);
}

// calculates the heliocentric coordinates of the planet at the specified time
Cord calcHeliocentricCord(const Planet &planet, float daysSinceEpoch) {
  const double normalizedMeanAnomaly =
      getNormalizedMeanAnomaly(planet, daysSinceEpoch);

  // orbital elements normalized to J2000
  const double a = planet.semiMajorAxis;
  const double e = planet.eccentricity;
  const double o = toRadians(planet.longitudeOfAscendingNode);
  const double p = toRadians(planet.longitudeOfPerihelion);
  const double i = toRadians(planet.orbitalInclination);

  // normalized to specified date
  const double M = toRadians(normalizedMeanAnomaly);
  const double E = calcEccentricAnomaly(e, M);

  // position in 2d orbital plane
  const double xv = a * (cos(E) - e);
  const double yv = a * (sqrt(1.0 - e * e) * sin(E));

  // The True Anomaly (v) is the final angle we need to define the position
  // of a satellite in orbit, the other two being Eccentric Anomaly (E) and
  // Mean Anomaly (M).
  const double v = atan2(yv, xv);

  // The radius vector (r) is the distance of the satellite to the focal point
  // of the ellipse, in this case the sun.
  const double r = sqrt(xv * xv + yv * yv);

  // heliocentric 3d cartesian coordinates
  const double xh =
      r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
  const double yh =
      r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
  const double zh = r * (sin(v + p - o) * sin(i));

  return {xh, yh, zh};
}

// creates a Waypoint struct that includes inputs and the results attributed to
// one travel entry. Acts as a controller
Waypoint createWaypoint(const Planet *&planets) {
  const Date date = getDate();
  const int planetIndex = getPlanetIndex();
  const double velocityAsMph = getVelocityAsMph();

  const float days = calcDaysSinceEpoch(date);

  const Planet planet = planets[planetIndex];
  const Planet earth = planets[2];

  Cord heliocentricCordPlanet = calcHeliocentricCord(planet, days);
  const Cord heliocentricCordEarth = calcHeliocentricCord(earth, days);

  // accounting for perturbations
  // const double meanAnomalyJupiter = getNormalizedMeanAnomaly(planets[4],
  // days); const double meanAnomalySaturn =
  // getNormalizedMeanAnomaly(planets[5], days); const double MeanAnomalyUranus
  // = getNormalizedMeanAnomaly(planets[6], days);

  // double lonecl = atan2(heliocentricCordPlanet.y, heliocentricCordPlanet.x);
  // double latecl =
  //     atan2(heliocentricCordPlanet.z,
  //           sqrt(heliocentricCordPlanet.x * heliocentricCordPlanet.x +
  //                heliocentricCordPlanet.y * heliocentricCordPlanet.y));

  // if (planet.name == "jupiter") {
  //   lonecl +=
  //       -0.332 * sin(2 * meanAnomalyJupiter - 5 * meanAnomalySaturn - 67.6);
  //   lonecl += -0.056 * sin(2 * meanAnomalyJupiter - 2 * meanAnomalySaturn +
  //   21); lonecl += 0.042 * sin(3 * meanAnomalyJupiter - 5 * meanAnomalySaturn
  //   + 21); lonecl += -0.036 * sin(meanAnomalyJupiter - 2 *
  //   meanAnomalySaturn); lonecl += 0.022 * cos(meanAnomalyJupiter -
  //   meanAnomalySaturn); lonecl += 0.023 * sin(2 * meanAnomalyJupiter - 3 *
  //   meanAnomalySaturn + 52); lonecl += -0.016 * sin(meanAnomalyJupiter - 5 *
  //   meanAnomalySaturn - 69);
  // }

  // if (planet.name == "saturn") {
  //   lonecl +=
  //       0.812 * sin(2 * meanAnomalyJupiter - 5 * meanAnomalySaturn - 67.6);
  //   lonecl += -0.229 * cos(2 * meanAnomalyJupiter - 4 * meanAnomalySaturn -
  //   2); lonecl += 0.119 * sin(meanAnomalyJupiter - 2 * meanAnomalySaturn -
  //   3); lonecl += 0.046 * sin(2 * meanAnomalyJupiter - 6 * meanAnomalySaturn
  //   - 69); lonecl += 0.014 * sin(meanAnomalyJupiter - 3 * meanAnomalySaturn +
  //   32);

  //   latecl += -0.020 * cos(2 * meanAnomalyJupiter - 4 * meanAnomalySaturn -
  //   2); latecl += 0.018 * sin(2 * meanAnomalyJupiter - 6 * meanAnomalySaturn
  //   - 49);
  // }

  // if (planet.name == "uranus") {
  //   lonecl += 0.040 * sin(meanAnomalySaturn - 2 * MeanAnomalyUranus + 6);
  //   lonecl += 0.035 * sin(meanAnomalySaturn - 3 * MeanAnomalyUranus + 33);
  //   lonecl += -0.015 * sin(meanAnomalyJupiter - MeanAnomalyUranus + 20);
  // }

  // heliocentricCordPlanet.x =
  //     heliocentricCordPlanet.r * cos(lonecl) * cos(latecl);
  // heliocentricCordPlanet.y =
  //     heliocentricCordPlanet.r * sin(lonecl) * cos(latecl);
  // heliocentricCordPlanet.z = heliocentricCordPlanet.r * sin(latecl);

  // geocentric 3d cartesian coordinates
  const double gX = heliocentricCordEarth.x - heliocentricCordPlanet.x;
  const double gY = heliocentricCordEarth.y - heliocentricCordPlanet.y;
  const double gZ = heliocentricCordEarth.z - heliocentricCordPlanet.z;

  const double distance = sqrt(pow(gX, 2) + pow(gY, 2) + pow(gZ, 2));
  const double years = calcYears(distance, velocityAsMph);
  const string totalTimeAsString = formatTimeResult("Travel time", years);

  print(totalTimeAsString);

  return {date, planet.name, velocityAsMph, distance, years};
}

int main() {
  const int ADD_WAYPOINT = 1;
  const int HISTORY = 2;
  const int TOTAL = 3;
  const int QUIT = 4;

  int menuChoice = 0;
  size_t numInputs = 0;
  size_t maxInputs = 10;

  const Planet *planets = populatePlanets();
  Waypoint *waypoints = new Waypoint[maxInputs];

  print("Planet Trip Calculator");

  do {
    menuChoice = getMenuChoice(QUIT);

    if (menuChoice == ADD_WAYPOINT) {
      if (numInputs == maxInputs) {
        expandArray<Waypoint>(waypoints, maxInputs);
      }

      waypoints[numInputs] = createWaypoint(planets);
      numInputs += 1;
    } else if (menuChoice == HISTORY) {
      printHistory(waypoints, numInputs);
    } else if (menuChoice == TOTAL) {
      printTotal(waypoints);
    }

  } while (menuChoice != QUIT);

  delete[] planets;
  delete[] waypoints;
  planets = nullptr;
  waypoints = nullptr;

  print("\nEnd program.");

  return 0;
}
