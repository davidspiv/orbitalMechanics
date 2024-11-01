#include <math.h>

#include <iomanip>
#include <iostream>

#include "./helpers/timer.h"

using namespace std;

void createFactorialCache(double *&factorialCache, size_t iterations) {
  factorialCache[0] = 1;
  for (size_t i = 1; i <= iterations; i++) {
    factorialCache[i] = i * factorialCache[i - 1];
  }
}

double taylorSeriesSine(double x, size_t n) {
  double result = 0;
  bool isAddOperation = n % 4 && n % 4 != 3;
  double factorialCache[n + 1];
  double *factorialCachePointer = factorialCache;

  createFactorialCache(factorialCachePointer, n);

  // Horner's method avoids pow() but requires us to evaluate the polynomial
  // from the highest degree first, hence the reverse loop and factorial cache
  for (int i = n; i > 0; i -= 1) {
    const double coefficient = (1 / factorialCache[i]);

    if (i % 2) {
      result += isAddOperation ? coefficient : -coefficient;
      isAddOperation = !isAddOperation;
    }

    result *= x;
  }

  return result;
}

int main() {
  const double RADIANS = -.34234;
  const int ITERATIONS = 5;
  const double approx = taylorSeriesSine(RADIANS, ITERATIONS);

  cout << setprecision(10) << "approx: " << approx << endl;
  cout << setprecision(10) << "answer: " << sin(RADIANS) << endl;

  return 0;
}
