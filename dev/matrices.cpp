#include <math.h>

#include <iostream>

using namespace std;

struct spatialVector {
  double x;
  double y;
  double z;
};

spatialVector crossProduct(const spatialVector& a, const spatialVector& b) {
  spatialVector result;
  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;

  return result;
}

int main() {
  const spatialVector vectorA = {3, -3, 1};
  const spatialVector vectorB = {4, 9, 2};

  const spatialVector resultVector = crossProduct(vectorA, vectorB);

  cout << "x: " << resultVector.x << endl;
  cout << "y: " << resultVector.y << endl;
  cout << "z: " << resultVector.z << endl;

  return 0;
}
